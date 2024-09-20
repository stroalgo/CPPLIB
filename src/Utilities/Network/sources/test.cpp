#include <iostream>

int test(void) {

  // std::cout << "test function" << std::endl;
  // char buf[1024];
  // // struct ifconf ifc;
  // struct ifreq *ifr;
  // int sck, nInterfaces;
  // int i;
  // unsigned char mac[6];
  // if (true)
    return 1;
  // sck = socket(AF_INET, SOCK_DGRAM, 0);
  // if(sck < 0)
  // {
  //     perror("socket");
  //     return 1;
  // }

  // ifc.ifc_len = sizeof(buf);
  // ifc.ifc_buf = buf;
  // if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
  // {
  //     perror("ioctl(SIOCGIFCONF)");
  //     return 1;
  // }

  // ifr = ifc.ifc_req;
  // nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
  // for(i = 0; i < nInterfaces; i++)
  // {
  //     struct ifreq *item = &ifr[i];
  //     printf("Interface Name = %s\nIP = %s\n",
  //            item->ifr_name,
  //            inet_ntoa(((struct sockaddr_in
  //            *)&item->ifr_addr)->sin_addr));

  //     ioctl(sck, SIOCGIFNETMASK, item);
  //     printf("SubNet Mask = %s\n", inet_ntoa(((struct sockaddr_in
  //     *)&item->ifr_netmask)->sin_addr));

  // ioctl(sck, SIOCGIFBRDADDR, item);
  //     printf("BroadCat Address = %s\n", inet_ntoa(((struct sockaddr_in
  //     *)&item->ifr_broadaddr)->sin_addr));

  //     ioctl(sck, SIOCGIFHWADDR, item);
  //     memcpy(mac, item->ifr_hwaddr.sa_data, 6);
  // printf("MAC:%02X:%02X:%02X:%02X:%02X:%02X\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  // }
  return 0;
}
