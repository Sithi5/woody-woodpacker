/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 12:12:08 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/26 12:22:41 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

// illegal funtion needed for debug
void print_loop(int fd) {
    char buf[11];
    printf("fd = %d \n", fd);
    while (read(fd, &buf, 10) > 0)
    {
        printf("%s", buf);
    }
}

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    if (ac != 2) {
        return 1;
    }
    int fd;
    if ((fd = open(av[1],O_RDONLY)) == -1) {
        return 1;
    }
    print_loop(fd);
    return 0;
}

