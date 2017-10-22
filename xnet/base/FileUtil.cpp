//
// Created by zhangkuo on 17-10-22.
//
#include <xnet/base/FileUtil.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

using namespace xnet;
using namespace FileUtil;

ReadSmallFile::ReadSmallFile(const string &fileName) :
    fd_(::open(fileName.c_str(), O_RDONLY | O_CLOEXEC)),
    errno_(0)
{
    buffer_[0] = '\0';
    if(fd_ < 0)
    {
        errno_ = errno;
    }
}
ReadSmallFile::~ReadSmallFile()
{
    if(fd_ > 0)
    {
        ::close(fd_);
    }
}

int ReadSmallFile::readToString(int maxSize, string* content, int64_t* fileSize, int64_t* modifyTime, int64_t* createTime)
{
    int errorNo = errno_;
    if(fd_ < 0) return errorNo;

    content->clear();
    if(fileSize != nullptr)
    {
        struct stat statBuf;
        if(::fstat(fd_, &statBuf) == 0)
        {
            if(S_ISREG(statBuf.st_mode))
            {
                *fileSize = statBuf.st_size;
                content->reserve(static_cast<size_t>(std::min(static_cast<int64_t>(maxSize), *fileSize)));
            }
            else if(S_ISDIR(statBuf.st_mode))
            {
                errorNo = EISDIR;
            }

            if(modifyTime != nullptr)
            {
                *modifyTime = statBuf.st_mtime;
            }

            if(createTime != nullptr)
            {
                *createTime = statBuf.st_ctime;
            }
        }
        else
        {
            errorNo = errno;
        }
    }

    while(content->size() < maxSize)
    {
        size_t toRead = std::min(maxSize - content->size(), sizeof(buffer_));
        ssize_t n = ::read(fd_, buffer_, toRead);
        if(n > 0)
        {
            content->append(buffer_, static_cast<size_t>(n));
        }
        else
        {
            if(n < 0)
            {
                errorNo = errno;
            }
            break;
        }
    }

    return errorNo;
}

int ReadSmallFile::readToBuffer(int* size)
{
    int errorNo = errno_;
    if(fd_ >= 0)
    {
        ssize_t n = ::pread(fd_, buffer_, sizeof(buffer_)-1, 0);
        if(n >= 0)
        {
            if(size != nullptr)
            {
                *size = static_cast<int>(n);
            }
            buffer_[n] = '\0';
        }
    }
    else
    {
        errorNo = errno;
    }

    return errorNo;
}