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

|CMD|Name|Symbol|Arguments|Capability|
|---|---|---|---|---|
EOF
            end

            if !last_count.nil? and count != last_count + 1
                readme << "|...|||||\n"
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
end

generate_command_types