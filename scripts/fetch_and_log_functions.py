#!/usr/bin/python
import os
import json
def file_it(path, data_file, data):
    if os.path.exists(path):
        if is_file_empty(data_file):
            with open(data_file, 'w') as f:
                f.write("[")
                json.dump(data, f)
                f.write("]")
        else:
            with open(data_file, 'a+') as f:
                # Move read cursor to the start of file.
                f.seek(0)
                # If file is not empty then append '\n'
                d = f.read()
                if len(d) > 0 :
                    f.seek(-1, os.SEEK_END)
                    f.truncate()#take last array closing bracket
                    f.write(",\n")
                # Append text at the end of file
                else:
                    # if file is empty, place open array bracket
                    f.write("[")
                json.dump(data, f)
                f.write("]")
    else:
        os.makedirs(path)
        with open(data_file, 'w+') as f:
            f.write("[")
            json.dump(data, f)
            f.write("]")
def is_file_empty(file_path):
    """ Check if file is empty by confirming if its size is 0 bytes"""
    # Check if file exist and it is empty
    return os.path.exists(file_path) and os.stat(file_path).st_size == 0
