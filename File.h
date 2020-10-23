//
// Created by Nathaniel Blair on 14/10/20.
//

#ifndef KASAI_FILE_H
#define KASAI_FILE_H

#include <unistd.h>
#include <string>
#include <map>
#include <fstream>
#include "EventEngine.h"
#include "TypedPrototype.h"

using std::string;
using std::map;

class File {
public:
    static map<string, File*> files;
    string filename;
    static string directory;
    static inline bool exists (const string& name) {
        return ( access( name.c_str(), F_OK ) != -1 );
    }

    static File* Submit(string filename, string contents) {
        File* result;
        if (exists(filename)) {
            if (files.find(filename) == files.end()) {
                result = new File();
                result->filename = filename;

                files[filename] = result;

                result->Submit(contents);

            }
            else {
                result = files[filename];
            }
        }
        else {
            EventEngine::Trigger("new file created",
                                 new TypedPrototype<string>(
                                   filename, ETYPE::STRING));

            result = new File();
            result->filename = filename;
            files[filename] = result;

            result->Submit(contents);
        }

        return result;
    }

    void Submit(string contents) {
        EventEngine::Trigger("file overwrite submitted",
                             new TypedPrototype<string>(
                               filename, ETYPE::STRING));
        std::ofstream outfile(filename, std::ofstream::trunc);

        outfile << contents;

        outfile.close();
    }

    void WriteLine(string line) {
        EventEngine::Trigger("file linewrite submitted",
                             new TypedPrototype<string>(
                               filename, ETYPE::STRING));
        std::ofstream outfile(filename);

        outfile << line << std::endl;

        outfile.close();
    }

};


#endif //KASAI_FILE_H
