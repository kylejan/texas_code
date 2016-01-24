#!/usr/bin/python
# -*- coding: utf-8 -*-

import re

def camelcase_to_underscore(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).upper()

message_types = []

message_file = './message.proto'
message_file_handler = open(message_file, 'r')
for line in message_file_handler:
    if 'message' in line:
        message_types.append(camelcase_to_underscore(line.split(' ')[1]))

message_types.sort()
message_enums = [(x, message_types.index(x) + 1) for x in message_types]

message_type_file = '../include/texas_code/protocol/message_type.h'
message_type_file_handler = open(message_type_file, 'w')

message_type_file_handler.write('#pragma once\n\n')
message_type_file_handler.write('#include <cstdint>\n\n')
message_type_file_handler.write('namespace texas_code { namespace protocol {\n\n')
message_type_file_handler.write('\tenum class MessageType : std::int32_t {\n\n')

message_type_file_handler.write('\t\t' + 'UNKNOWN_REQUEST' + ' = 0,\n')
for enum in message_enums:
    message_type_file_handler.write('\t\t' + enum[0] + ' = ' + str(enum[1]) + ',\n')

message_type_file_handler.write('\t};\n}}\n')
message_type_file_handler.flush()
message_type_file_handler.close()
