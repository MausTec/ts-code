#!/usr/bin/env ruby

require 'json'

PWD = File.expand_path(File.join(File.dirname(__FILE__), ".."))

def generate_command_types
    command_types_json = File.join(PWD, "data", "tscode_command_types.json");
    data = JSON.parse(File.read(command_types_json), symbolize_names: true)

    readme = <<-EOF
# TS-code Listing

**T-codes**

|CMD|Name|Symbol|Arguments|Capability|
|---|---|---|---|---|
EOF

    data[:commands].select { |d| d[:command] =~ /^T/ }.each do |command|
        args = []
        command[:arguments].scan(/\[?[A-Z]\]?/).each do |arg|
            if arg =~ /\[(\S)\]/
                args << "#{$1}?"
            else
                args << arg
            end
        end
        
        readme << "|%{command}|%{name}|`%{symbol}`|" % command
        readme << args.join(', ') << "|`#{command[:capability_group]}`|\n"
    end

    readme << <<-EOF

\? denotes an optional argument.
EOF

    puts readme
end

generate_command_types