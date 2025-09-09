###clone OS2 labs

```
git clone https://gitlab.cs.pub.ro/so2/so2-labs.git
```
##install docker (for linux)

```
 https://docs.docker.com/engine/install/ubuntu/
 #if you are on windows , you can run WSL and then install docker inside it or simply use a virtuzl machine
```
##change your direction to so2-labs/

```
cd so2-labs/
```

##create a docker container

```
sudo ./local.sh docker interactive --privileged
```

##create a shared folder between Qemu and Docker 

```
LABS=kernel_modules make skels
```

##Boot Qemu 

```
make console
```
##write root in login 

```
qemux86 login:root
```

##go to skels and insert the module 

```
root@qemux86:~/skels/lab02# insmod linked_list.ko                                          
module init                                                                                
added pid : 1111 using spinlock                                                            
added pid: 2222                                                                            
added pid: 3333                                                                            
Current list:                                                                              
PID :3333                                                                                  
PID :2222                                                                                  
PID :1111                         
```


##useful links : 

https://linux-kernel-labs.github.io/refs/heads/master/index.html
https://embetronicx.com/tutorials/linux/device-drivers/setup-ubuntu-and-raspberry-pi-linux-device-driver-tutorial/
https://www.youtube.com/watch?v=n9CNgMlxMyQ&list=PLNpnO_Q_GAdQ4eVO0Lp0IxNvcgvZZ-J22


