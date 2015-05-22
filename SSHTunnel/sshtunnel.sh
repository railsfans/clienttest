#! /bin/sh

. config.sh

#SSHCMD="ssh -NfR 60.12.182.102:1572:localhost:22 -o ServerAliveInterval=10 root@60.12.182.102"
SSHCMD="ssh -NfR ${RemoteIp}:${RemotePort}:${LocalIp}:${LocalPort} -o ServerAliveInterval=${AliveInterval} root@${RemoteIp}"

function make_proxy()
{
        PROXY_ID=$(pgrep -f "$SSHCMD" | wc -l)
        if [[ $PROXY_ID == 0 ]];then
                echo "$SSHCMD"
                $SSHCMD >/tmp/ssh.log 2>&1
                # handle some exception "Warning: remote port forwarding failed"
                sleep 10
                result=$(grep "failed" /tmp/ssh.log | wc -l)
                if [ $result != 0 ]; then
                        echo "$(grep "failed" /tmp/ssh.log)"
                        echo "killall ssh"
                        killall ssh;
                fi
        else
                echo "Alive"
        fi
}

while true;
do
        make_proxy;
        sleep ${AliveInterval};
done
