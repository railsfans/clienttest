#! /bin/sh

RemoteIp=60.12.182.102
RemotePort=1575
Comments="ADDE-XX-XX-XX-XX-XX-XXXX"
LocalIp=127.0.0.1
LocalPort=22
AliveInterval=10


read_string()
{
    echo   -n   $1 
    read   inputstr
}

inputstr=""

# get remote ip 
read_string "Enter remote ip[default: 60.12.182.102 ]:" 
if [ "$inputstr" != "" ]; then
	echo "$inputstr"
	RemoteIp=$inputstr
fi

# get remote port
read_string "Enter remote port:"
if [ "$inputstr" != "" ]; then
	RemotePort=$inputstr
fi

# get comments
read_string "Enter comments:"
if [ "$inputstr" != "" ]; then
        Comments=$inputstr
fi

# generate config.sh
echo "#! /bin/sh"				>config.sh
echo "RemoteIp=$RemoteIp"		>>config.sh
echo "RemotePort=$RemotePort"	>>config.sh
echo "LocalIp=$LocalIp"			>>config.sh
echo "LocalPort=$LocalPort"		>>config.sh
echo "AliveInterval=$AliveInterval"	>>config.sh
echo "Comments=$Comments"		>>config.sh

echo "##########################################"
cat config.sh

# generate and copy ssh key
ssh-keygen -t rsa -N "" -C root@${Comments} -f ~/.ssh/id_rsa
ssh-copy-id -i ~/.ssh/id_rsa.pub $RemoteIp

# mv SSHTunnel to /opt/SSHTunnel
cur_dir=$(pwd)
if [ "$cur_dir" != "/opt/SSHTunnel" ]; then
	rm -rf /opt/SSHTunnel
	mv $cur_dir /opt/SSHTunnel
fi

# add cmd to /etc/rc.local
exist_line=$(grep "./sshtunnel.sh" /etc/rc.local | wc -l)
if [ $exist_line == 0 ]; then
	echo "cd /opt/SSHTunnel/"							>>/etc/rc.local
	echo "./sshtunnel.sh >/tmp/sshtunnel.log 2>&1 &"	>>/etc/rc.local
fi

# start sshtunnel
killall sshtunnel.sh
killall ssh
cd /opt/SSHTunnel/
nohup ./sshtunnel.sh >/tmp/sshtunnel.log 2>&1 &






