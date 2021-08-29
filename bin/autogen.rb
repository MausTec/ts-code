#!/usr/bin/env ruby

require 'json'

PWD = File.expand_path(File.join(File.dirname(__FILE__), ".."))

def generate_md_table(rows, *cols)
    last_num = -1
    rows.collect do |row|
        num = row[cols[0]].gsub(/[^\d]/, '').to_i
        prefix = "|"
        if num != last_num + 1
            prefix += "..." + ("|" * cols.count) + "\n|"
        end
        last_num = num
        prefix + row.slice(*cols).values.join("|") + "|\n"
    end.join
end

def generate_command_types
    command_types_json = File.join(PWD, "data", "tscode_command_types.json");
    data = JSON.parse(File.read(command_types_json))

    readme = <<-EOF
# TS-code Listing

**T-codes**

|CMD|Name|Symbol|Description|
|---|----|------|-----------|
#{generate_md_table data["commands"].select { |d| d["command"] =~ /^T/ }, 
    "command", "name", "symbol", "description"
}

**S-codes**

|CMD|Name|Symbol|Description|
|---|----|------|-----------|
#{generate_md_table data["commands"].select { |d| d["command"] =~ /^S/ }, 
    "command", "name", "symbol", "description"
}
EOF

    puts readme
end

generate_command_types