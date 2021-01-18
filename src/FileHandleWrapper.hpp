#pragma once
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>

namespace webcam {

class FileHandleWrapper {
public:
    FileHandleWrapper(const std::string &path, int flags) {
        handle = open(path.c_str(), flags);
        if (handle <= -1)
        {
            throw std::runtime_error("Unable to open filehandle");
        }
    }

    ~FileHandleWrapper() {
        if (handle > 0) {
            close(handle);
        }
    }

    FileHandleWrapper(const FileHandleWrapper &) = delete;
    FileHandleWrapper(FileHandleWrapper &&) = delete;
    FileHandleWrapper& operator=(const FileHandleWrapper&) = delete;
    FileHandleWrapper& operator=(FileHandleWrapper&&) = delete;

    int get() {
        return handle;
    }
private:
    int handle;
};

}

