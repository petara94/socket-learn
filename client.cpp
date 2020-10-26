#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
using namespace std;

int main()
{
    char *message;
    char end[] = "";
    string input;

    cout << "send text: ";
    getline(cin, input);

    message = new char[input.size()];
    char *tmp = (char *)input.c_str();

    for (int i = 0; i < input.size(); i++)
        message[i] = tmp[i];

    struct sockaddr_in addr;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(2345); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    send(sock, message, input.size(), 0);
    //send(sock, end, sizeof(end), 0);

    delete[] message;

    return 0;
}