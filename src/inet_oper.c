//
// Created by smitropoulos on 29/07/2019.
//

#include "inet_oper.h"
#include "inet_sockets.h"

/*!
 * Broadcast a magic packet to 255.255.255.255. The packet with the physical address is created elsewhere
 * @param packet The packet to be broadcast (in general a magic WOL packet)
 * @return Returns 0 on success, -1 on errors
 */
int sendWOLPacket(unsigned char *packet) {

    int sock;
    int optval = 1;
    struct sockaddr_in addr;
    const char broadcast_addr[] = "255.255.255.255";

    addr.sin_family = AF_INET;
    addr.sin_port = htons(WOL_PORT);

    if (inet_aton(broadcast_addr, &addr.sin_addr) == 0) {
        perror("Not valid IP address given");
        return -1;
    }

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Problem in creating a socket!");
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof(optval)) < 0) {
        perror("Problem in setting socket options!");
        return -1;
    }

    if (sendto(sock, packet, MAGIC_PACKET_LENGTH, 0, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Cannot send data!");
        return -1;
    }

    return 0;
}

/*!
 * Resolve the host name and ip address of the host and print them on stdout
 * @return Returns 0 on success, 1 on errors
 */
int hostResolve() {

    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;

    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname == -1) {
        perror("gethostname");
        return (1);
    }
    // To retrieve host information
    host_entry = gethostbyname(hostbuffer);
    if (host_entry == NULL) {
        perror("gethostbyname");
        return (1);
    }
    // To convert an Internet network
    // address into ASCII string
    IPbuffer = inet_ntoa(*((struct in_addr *) host_entry->h_addr_list[0]));

    printf("Hostname: %s\n", hostbuffer);
    printf("Host IP: %s\n", IPbuffer);

    return 0;
}
