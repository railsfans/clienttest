基站远程登录不上的原因找到了。
通过 tail -n 100 -f /var/log/secure查看sshd的日志，发现如下类型信息
Accepted publickey for root from 58.100.87.46 port 51983 ssh2
Feb 18 15:12:27 localhost sshd[7704]: pam_unix(sshd:session): session opened for user root by (uid=0)
Feb 18 15:12:27 localhost sshd[7704]: error: bind: Address already in use
Feb 18 15:12:27 localhost sshd[7704]: error: channel_setup_fwd_listener: cannot listen to port: 1575
查看进程
[root@localhost ~]# netstat -nltup | grep sshd
tcp        0      0 :::1572                     :::*                        LISTEN      7304/sshd: root
tcp        0      0 :::1575                     :::*                        LISTEN      7791/sshd: root
tcp        0      0 :::50001                    :::*                        LISTEN      6725/sshd: root
tcp        0      0 :::50002                    :::*                        LISTEN      30511/sshd: root
tcp        0      0 :::22                       :::*                        LISTEN      2658/sshd

此时端口1575对应的机制就无法和中心建立连接。因此也不能登录到该基站了。

临时解决方法：
kill 7791杀死对应进程即可。


