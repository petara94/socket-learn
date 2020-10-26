#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <unistd.h>
using namespace std;

void fill(char *arr, int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = '\0';
}

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    int bytes_read;
    bool work = true;
    int port;

    cout << "port: ";
    cin >> port;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("192.168.1.2");
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);

    while (work)
    {
        sock = accept(listener, NULL, NULL);
        if (sock < 0)
        {
            perror("accept");
            exit(3);
        }

        while (1)
        {
            char buf[1024];
            fill(buf, 1024);
            bytes_read = recv(sock, buf, 1024, 0);
            if (bytes_read <= 0)
                break;
            string pkg(buf);
            vector<string> v;
            boost::split(v, pkg, boost::is_any_of(" "));
            cout << "<pkg>\n"
                 << pkg << "\n</pkg>\n"
                 << endl;
            if (v[0] == "end")
            {
                work = false;
                cout << "Good bay!" << endl;
            }
            else if (v[0] == "write")
            {
                if (v.size() < 3)
                    break;
                ofstream f(v[1]);
                for (int i = 2; i < v.size(); i++)
                {
                    f.write(v[i].c_str(), v[i].size());
                    f.write(" ", 1);
                }
                f.close();
            }
        }

        close(sock);
    }
    close(listener);
}