#!/usr/bin/env ruby

require 'json'
require 'fileutils'

PWD = File.expand_path(File.join(File.dirname(__FILE__), ".."))

def generate_command_types
    command_types_json = File.join(PWD, "data", "tscode_command_types.json");
    data = JSON.parse(File.read(command_types_json), symbolize_names: true)

    readme = <<-EOF
# TS-code Commands
EOF

    last_group = nil;
    last_count = nil;

    data[:commands].each do |command|
        args = []

        if command[:command] =~ /([TS])(\d+)/
            group = $1
            count = $2.to_i
            if last_group != group
                last_count = nil
                readme << <<-EOF

**#{group}-codes**

|CMD|Name|Symbol|Arguments|
|---|---|---|---|
EOF
            end

            if !last_count.nil? and count != last_count + 1
                readme << "|...||||\n"
            end

            last_group = group
            last_count = count
        end

        command[:arguments].scan(/\[?[A-Z]\]?/).each do |arg|
            optional = false
            if arg =~ /\[(\S)\]/
                optional = true
                arg = $1
            end

            args << "[`%s`%s](#tscode-argument-%s)" % [
                arg, optional ? '?' : '', arg.downcase
            ]
        end
        
        readme << "|[%{command}](#tscode-command-%{command})|%{name}|`%{symbol}`|" % command
        readme << args.join(', ') << "|\n"
    end

    readme << <<-EOF

\? denotes an optional argument.

EOF

    readme_filename = File.join(PWD, "README.md")
    readme_txt = File.readlines(readme_filename)
    FileUtils.mv(readme_filename, readme_filename + ".bak")
    
    fh = File.open(readme_filename, 'w+')
    in_cmd_list = false
    readme_txt.each do |line|
        if in_cmd_list && line =~ /^#/
            in_cmd_list = false
        end

        if line =~ /^#\s+TS-code\s+Commands/i
            in_cmd_list = true
            fh.puts readme
        elsif !in_cmd_list
            fh.puts line
        end
    end
    fh.close

    # Generate Commands from TS-code
    tscode_command_types = <<-EOF
/**
 * THIS IS AN AUTOMATICALLY GENERATED FILE
 * DO NOT EDIT THIS BY HAND
 * 
 * This is created by bin/autogen.rb
 */
#ifndef __tscode_command_types_h
#define __tscode_command_types_h

#ifdef __cplusplus
extern "C" {
#endif

#define __T(x) (0000 + x)
#define __S(x) (1000 + x)

/**
 * For this enumeration, T-codes occupy 0-999, and S-codes occupy 1000-1999
 */

enum tscode_command_type {
    TSCODE_COMMAND_INVALID = -1,
    EOF
        
    data[:commands].each do |command|
        tscode_command_types << "    TSCODE_#{command[:symbol]} = __#{command[:command][0]}(#{command[:command][1..-1].to_i}),\n"
    end

    tscode_command_types << <<-EOF
};

typedef enum tscode_command_type tscode_command_type_t;

#ifdef __cplusplus
}
#endif

#endif
EOF

    tscode_command_types_path = File.join(PWD, "src", "tscode_command_types.h")
    FileUtils.mv(tscode_command_types_path, tscode_command_types_path + ".bak")
    File.write(tscode_command_types_path, tscode_command_types)
end

generate_command_types