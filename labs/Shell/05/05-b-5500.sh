#!/bin/bash

cat /etc/passwd |\
        awk -F : 'BEGIN { printf("<table>\n <tr>\n  <th>%s</th>\n  <th>%s</th>\n  <th>%s</th>\n  <th>%s</th>\n </tr>\n",
        "Username", "group", "login shell", "GECKO") } 
        { printf(" <tr>\n  <td>%s</td>\n  <td>%s</td>\n  <td>%s</td>\n  <td>%s</td>\n </tr>\n", $1, $4, $5, $7) } 
        END { printf("</table>\n") }'
