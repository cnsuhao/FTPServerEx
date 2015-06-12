#include <iostream>
#include "FtpServer.h"
#include "CommonFile.h"

using namespace std;

int main()
{
    char buf[1024];
    FtpServer ftp("0.0.0.0", 8000);
    cout << ftp.run() << endl;
    cout << buf << endl;
    /*
    cout << CommonFile::makePath(buf, "/home", "test") << endl;
    cout << CommonFile::isChildDir(buf, "/home/test/sen") << endl;
    */
    return 0;
}

