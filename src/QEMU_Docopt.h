/****************************************************************************
**
** Copyright (C) 2016 Tobias Gläßer
**
** This file is part of AQEMU.
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor,
** Boston, MA  02110-1301, USA.
**
****************************************************************************/

#include <QString>

#include "docopt/docopt.h"

static const char QEMU_USAGE[] =
R"(QEMU emulator version 3.1.0
Copyright (c) 2003-2018 Fabrice Bellard and the QEMU Project developers
usage: qemu-system-x86_64 [options] [disk_image]

'disk_image' is a raw hard disk image for IDE hard disk 0

Standard options:
-h or -help     display this help and exit
-version        display version information and exit
-machine [type=]name[,prop[=value][,...]]
                selects emulated machine ('-machine help' for list)
                property accel=accel1[:accel2[:...]] selects accelerator
                supported accelerators are kvm, xen, hax, hvf, whpx or tcg (default: tcg)
                kernel_irqchip=on|off|split controls accelerated irqchip support (default=off)
                vmport=on|off|auto controls emulation of vmport (default: auto)
                kvm_shadow_mem=size of KVM shadow MMU in bytes
                dump-guest-core=on|off include guest memory in a core dump (default=on)
                mem-merge=on|off controls memory merge support (default: on)
                igd-passthru=on|off controls IGD GFX passthrough support (default=off)
                aes-key-wrap=on|off controls support for AES key wrapping (default=on)
                dea-key-wrap=on|off controls support for DEA key wrapping (default=on)
                suppress-vmdesc=on|off disables self-describing migration (default=off)
                nvdimm=on|off controls NVDIMM support (default=off)
                enforce-config-section=on|off enforce configuration section migration (default=off)
                memory-encryption=@var{} memory encryption object to use (default=none)
-cpu cpu        select CPU ('-cpu help' for list)
-accel [accel=]accelerator[,thread=single|multi]
                select accelerator (kvm, xen, hax, hvf, whpx or tcg; use 'help' for a list)
                thread=single|multi (enable multi-threaded TCG)
-smp [cpus=]n[,maxcpus=cpus][,cores=cores][,threads=threads][,sockets=sockets]
                set the number of CPUs to 'n' [default=1]
                maxcpus= maximum number of total cpus, including
                offline CPUs for hotplug, etc
                cores= number of CPU cores on one socket
                threads= number of threads on one CPU core
                sockets= number of discrete sockets in the system
-numa node[,mem=size][,cpus=firstcpu[-lastcpu]][,nodeid=node]
-numa node[,memdev=id][,cpus=firstcpu[-lastcpu]][,nodeid=node]
-numa dist,src=source,dst=destination,val=distance
-numa cpu,node-id=node[,socket-id=x][,core-id=y][,thread-id=z]
-add-fd fd=fd,set=set[,opaque=opaque]
                Add 'fd' to fd 'set'
-set group.id.arg=value
                set <arg> parameter for item <id> of type <group>
                i.e. -set drive.$id.file=/path/to/image
-global driver.property=value
-global driver=driver,property=property,value=value
                set a global default for a driver property
-boot [order=drives][,once=drives][,menu=on|off]
      [,splash=sp_name][,splash-time=sp_time][,reboot-timeout=rb_time][,strict=on|off]
                'drives': floppy (a), hard disk (c), CD-ROM (d), network (n)
                'sp_name': the file's name that would be passed to bios as logo picture, if menu=on
                'sp_time': the period that splash picture last if menu=on, unit is ms
                'rb_timeout': the timeout before guest reboot when boot failed, unit is ms
-m [size=]megs[,slots=n,maxmem=size]
                configure guest RAM
                size: initial amount of guest memory
                slots: number of hotplug slots (default: none)
                maxmem: maximum amount of guest memory (default: none)
NOTE: Some architectures might enforce a specific granularity
-mem-path FILE  provide backing storage for guest RAM
-mem-prealloc   preallocate guest memory (use with -mem-path)
-k language     use keyboard layout (for example 'fr' for French)
-audio-help     print list of audio drivers and their options
-soundhw c1,... enable audio support
                and only specified sound cards (comma separated list)
                use '-soundhw help' to get the list of supported cards
                use '-soundhw all' to enable all of them
-device driver[,prop[=value][,...]]
                add device (based on driver)
                prop=value,... sets driver properties
                use '-device help' to print all possible drivers
                use '-device driver,help' to print all possible properties
-name string1[,process=string2][,debug-threads=on|off]
                set the name of the guest
                string1 sets the window title and string2 the process name (on Linux)
                When debug-threads is enabled, individual threads are given a separate name (on Linux)
                NOTE: The thread names are for debugging and not a stable API.
-uuid %08x-%04x-%04x-%04x-%012x
                specify machine UUID

Block device options:
-fda/-fdb file  use 'file' as floppy disk 0/1 image
-hda/-hdb file  use 'file' as IDE hard disk 0/1 image
-hdc/-hdd file  use 'file' as IDE hard disk 2/3 image
-cdrom file     use 'file' as IDE cdrom image (cdrom is ide1 master)
-blockdev [driver=]driver[,node-name=N][,discard=ignore|unmap]
          [,cache.direct=on|off][,cache.no-flush=on|off]
          [,read-only=on|off][,detect-zeroes=on|off|unmap]
          [,driver specific parameters...]
                configure a block backend
-drive [file=file][,if=type][,bus=n][,unit=m][,media=d][,index=i]
       [,cache=writethrough|writeback|none|directsync|unsafe][,format=f]
       [,snapshot=on|off][,rerror=ignore|stop|report]
       [,werror=ignore|stop|report|enospc][,id=name][,aio=threads|native]
       [,readonly=on|off][,copy-on-read=on|off]
       [,discard=ignore|unmap][,detect-zeroes=on|off|unmap]
       [[,bps=b]|[[,bps_rd=r][,bps_wr=w]]]
       [[,iops=i]|[[,iops_rd=r][,iops_wr=w]]]
       [[,bps_max=bm]|[[,bps_rd_max=rm][,bps_wr_max=wm]]]
       [[,iops_max=im]|[[,iops_rd_max=irm][,iops_wr_max=iwm]]]
       [[,iops_size=is]]
       [[,group=g]]
                use 'file' as a drive image
-mtdblock file  use 'file' as on-board Flash memory image
-sd file        use 'file' as SecureDigital card image
-pflash file    use 'file' as a parallel flash image
-snapshot       write to temporary files instead of disk image files
-fsdev fsdriver,id=id[,path=path,][security_model={mapped-xattr|mapped-file|passthrough|none}]
 [,writeout=immediate][,readonly][,socket=socket|sock_fd=sock_fd][,fmode=fmode][,dmode=dmode]
 [[,throttling.bps-total=b]|[[,throttling.bps-read=r][,throttling.bps-write=w]]]
 [[,throttling.iops-total=i]|[[,throttling.iops-read=r][,throttling.iops-write=w]]]
 [[,throttling.bps-total-max=bm]|[[,throttling.bps-read-max=rm][,throttling.bps-write-max=wm]]]
 [[,throttling.iops-total-max=im]|[[,throttling.iops-read-max=irm][,throttling.iops-write-max=iwm]]]
 [[,throttling.iops-size=is]]
-virtfs local,path=path,mount_tag=tag,security_model=[mapped-xattr|mapped-file|passthrough|none]
        [,id=id][,writeout=immediate][,readonly][,socket=socket|sock_fd=sock_fd][,fmode=fmode][,dmode=dmode]
-virtfs_synth Create synthetic file system image
-iscsi [user=user][,password=password]
       [,header-digest=CRC32C|CR32C-NONE|NONE-CRC32C|NONE
       [,initiator-name=initiator-iqn][,id=target-iqn]
       [,timeout=timeout]
                iSCSI session parameters

USB options:
-usb            enable the USB driver (if it is not used by default yet)
-usbdevice name add the host or guest USB device 'name'

Display options:
-display sdl[,frame=on|off][,alt_grab=on|off][,ctrl_grab=on|off]
            [,window_close=on|off][,gl=on|core|es|off]
-display gtk[,grab_on_hover=on|off][,gl=on|off]|
-display vnc=<display>[,<optargs>]
-display curses
-display none
-display egl-headless[,rendernode=<file>]                select display type
The default display is equivalent to
    '-display gtk'
-nographic      disable graphical output and redirect serial I/Os to console
-curses         shorthand for -display curses
-no-frame       open SDL window without a frame and window decorations
-alt-grab       use Ctrl-Alt-Shift to grab mouse (instead of Ctrl-Alt)
-ctrl-grab      use Right-Ctrl to grab mouse (instead of Ctrl-Alt)
-no-quit        disable SDL window close capability
-sdl            shorthand for -display sdl
-spice [port=port][,tls-port=secured-port][,x509-dir=<dir>]
       [,x509-key-file=<file>][,x509-key-password=<file>]
       [,x509-cert-file=<file>][,x509-cacert-file=<file>]
       [,x509-dh-key-file=<file>][,addr=addr][,ipv4|ipv6|unix]
       [,tls-ciphers=<list>]
       [,tls-channel=[main|display|cursor|inputs|record|playback]]
       [,plaintext-channel=[main|display|cursor|inputs|record|playback]]
       [,sasl][,password=<secret>][,disable-ticketing]
       [,image-compression=[auto_glz|auto_lz|quic|glz|lz|off]]
       [,jpeg-wan-compression=[auto|never|always]]
       [,zlib-glz-wan-compression=[auto|never|always]]
       [,streaming-video=[off|all|filter]][,disable-copy-paste]
       [,disable-agent-file-xfer][,agent-mouse=[on|off]]
       [,playback-compression=[on|off]][,seamless-migration=[on|off]]
       [,gl=[on|off]][,rendernode=<file>]
   enable spice
   at least one of {port, tls-port} is mandatory
-portrait       rotate graphical output 90 deg left (only PXA LCD)
-rotate <deg>   rotate graphical output some deg left (only PXA LCD)
-vga [std|cirrus|vmware|qxl|xenfb|tcx|cg3|virtio|none]
                select video card type
-full-screen    start in full screen
-vnc <display>  shorthand for -display vnc=<display>

i386 target only:
-win2k-hack     use it when installing Windows 2000 to avoid a disk full bug
-no-fd-bootchk  disable boot signature checking for floppy disks
-no-acpi        disable ACPI
-no-hpet        disable HPET
-acpitable [sig=str][,rev=n][,oem_id=str][,oem_table_id=str][,oem_rev=n][,asl_compiler_id=str][,asl_compiler_rev=n][,{data|file}=file1[:file2]...]
                ACPI table description
-smbios file=binary
                load SMBIOS entry from binary file
-smbios type=0[,vendor=str][,version=str][,date=str][,release=%d.%d]
              [,uefi=on|off]
                specify SMBIOS type 0 fields
-smbios type=1[,manufacturer=str][,product=str][,version=str][,serial=str]
              [,uuid=uuid][,sku=str][,family=str]
                specify SMBIOS type 1 fields
-smbios type=2[,manufacturer=str][,product=str][,version=str][,serial=str]
              [,asset=str][,location=str]
                specify SMBIOS type 2 fields
-smbios type=3[,manufacturer=str][,version=str][,serial=str][,asset=str]
              [,sku=str]
                specify SMBIOS type 3 fields
-smbios type=4[,sock_pfx=str][,manufacturer=str][,version=str][,serial=str]
              [,asset=str][,part=str]
                specify SMBIOS type 4 fields
-smbios type=17[,loc_pfx=str][,bank=str][,manufacturer=str][,serial=str]
               [,asset=str][,part=str][,speed=%d]
                specify SMBIOS type 17 fields

Network options:
-netdev user,id=str[,ipv4[=on|off]][,net=addr[/mask]][,host=addr]
         [,ipv6[=on|off]][,ipv6-net=addr[/int]][,ipv6-host=addr]
         [,restrict=on|off][,hostname=host][,dhcpstart=addr]
         [,dns=addr][,ipv6-dns=addr][,dnssearch=domain][,domainname=domain]
         [,tftp=dir][,tftp-server-name=name][,bootfile=f][,hostfwd=rule][,guestfwd=rule][,smb=dir[,smbserver=addr]]
                configure a user mode network backend with ID 'str',
                its DHCP server and optional services
-netdev tap,id=str[,fd=h][,fds=x:y:...:z][,ifname=name][,script=file][,downscript=dfile]
         [,br=bridge][,helper=helper][,sndbuf=nbytes][,vnet_hdr=on|off][,vhost=on|off]
         [,vhostfd=h][,vhostfds=x:y:...:z][,vhostforce=on|off][,queues=n]
         [,poll-us=n]
                configure a host TAP network backend with ID 'str'
                connected to a bridge (default=br0)
                use network scripts 'file' (default=/etc/qemu-ifup)
                to configure it and 'dfile' (default=/etc/qemu-ifdown)
                to deconfigure it
                use '[down]script=no' to disable script execution
                use network helper 'helper' (default=/usr/lib/qemu/qemu-bridge-helper) to
                configure it
                use 'fd=h' to connect to an already opened TAP interface
                use 'fds=x:y:...:z' to connect to already opened multiqueue capable TAP interfaces
                use 'sndbuf=nbytes' to limit the size of the send buffer (the
                default is disabled 'sndbuf=0' to enable flow control set 'sndbuf=1048576')
                use vnet_hdr=off to avoid enabling the IFF_VNET_HDR tap flag
                use vnet_hdr=on to make the lack of IFF_VNET_HDR support an error condition
                use vhost=on to enable experimental in kernel accelerator
                    (only has effect for virtio guests which use MSIX)
                use vhostforce=on to force vhost on for non-MSIX virtio guests
                use 'vhostfd=h' to connect to an already opened vhost net device
                use 'vhostfds=x:y:...:z to connect to multiple already opened vhost net devices
                use 'queues=n' to specify the number of queues to be created for multiqueue TAP
                use 'poll-us=n' to speciy the maximum number of microseconds that could be
                spent on busy polling for vhost net
-netdev bridge,id=str[,br=bridge][,helper=helper]
                configure a host TAP network backend with ID 'str' that is
                connected to a bridge (default=br0)
                using the program 'helper (default=/usr/lib/qemu/qemu-bridge-helper)
-netdev l2tpv3,id=str,src=srcaddr,dst=dstaddr[,srcport=srcport][,dstport=dstport]
         [,rxsession=rxsession],txsession=txsession[,ipv6=on/off][,udp=on/off]
         [,cookie64=on/off][,counter][,pincounter][,txcookie=txcookie]
         [,rxcookie=rxcookie][,offset=offset]
                configure a network backend with ID 'str' connected to
                an Ethernet over L2TPv3 pseudowire.
                Linux kernel 3.3+ as well as most routers can talk
                L2TPv3. This transport allows connecting a VM to a VM,
                VM to a router and even VM to Host. It is a nearly-universal
                standard (RFC3391). Note - this implementation uses static
                pre-configured tunnels (same as the Linux kernel).
                use 'src=' to specify source address
                use 'dst=' to specify destination address
                use 'udp=on' to specify udp encapsulation
                use 'srcport=' to specify source udp port
                use 'dstport=' to specify destination udp port
                use 'ipv6=on' to force v6
                L2TPv3 uses cookies to prevent misconfiguration as
                well as a weak security measure
                use 'rxcookie=0x012345678' to specify a rxcookie
                use 'txcookie=0x012345678' to specify a txcookie
                use 'cookie64=on' to set cookie size to 64 bit, otherwise 32
                use 'counter=off' to force a 'cut-down' L2TPv3 with no counter
                use 'pincounter=on' to work around broken counter handling in peer
                use 'offset=X' to add an extra offset between header and data
-netdev socket,id=str[,fd=h][,listen=[host]:port][,connect=host:port]
                configure a network backend to connect to another network
                using a socket connection
-netdev socket,id=str[,fd=h][,mcast=maddr:port[,localaddr=addr]]
                configure a network backend to connect to a multicast maddr and port
                use 'localaddr=addr' to specify the host address to send packets from
-netdev socket,id=str[,fd=h][,udp=host:port][,localaddr=host:port]
                configure a network backend to connect to another network
                using an UDP tunnel
-netdev vde,id=str[,sock=socketpath][,port=n][,group=groupname][,mode=octalmode]
                configure a network backend to connect to port 'n' of a vde switch
                running on host and listening for incoming connections on 'socketpath'.
                Use group 'groupname' and mode 'octalmode' to change default
                ownership and permissions for communication port.
-netdev vhost-user,id=str,chardev=dev[,vhostforce=on|off]
                configure a vhost-user network, backed by a chardev 'dev'
-netdev hubport,id=str,hubid=n[,netdev=nd]
                configure a hub port on the hub with ID 'n'
-nic [tap|bridge|user|l2tpv3|vde|vhost-user|socket][,option][,...][mac=macaddr]
                initialize an on-board / default host NIC (using MAC address
                macaddr) and connect it to the given host network backend
-nic none       use it alone to have zero network devices (the default is to
                provided a 'user' network connection)
-net nic[,macaddr=mac][,model=type][,name=str][,addr=str][,vectors=v]
                configure or create an on-board (or machine default) NIC and
                connect it to hub 0 (please use -nic unless you need a hub)
-net [user|tap|bridge|vde|socket][,option][,option][,...]
                old way to initialize a host network interface
                (use the -netdev option if possible instead)

Character device options:
-chardev help
-chardev null,id=id[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev socket,id=id[,host=host],port=port[,to=to][,ipv4][,ipv6][,nodelay][,reconnect=seconds]
         [,server][,nowait][,telnet][,websocket][,reconnect=seconds][,mux=on|off]
         [,logfile=PATH][,logappend=on|off][,tls-creds=ID] (tcp)
-chardev socket,id=id,path=path[,server][,nowait][,telnet][,websocket][,reconnect=seconds]
         [,mux=on|off][,logfile=PATH][,logappend=on|off] (unix)
-chardev udp,id=id[,host=host],port=port[,localaddr=localaddr]
         [,localport=localport][,ipv4][,ipv6][,mux=on|off]
         [,logfile=PATH][,logappend=on|off]
-chardev msmouse,id=id[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev vc,id=id[[,width=width][,height=height]][[,cols=cols][,rows=rows]]
         [,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev ringbuf,id=id[,size=size][,logfile=PATH][,logappend=on|off]
-chardev file,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev pipe,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev pty,id=id[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev stdio,id=id[,mux=on|off][,signal=on|off][,logfile=PATH][,logappend=on|off]
-chardev serial,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev tty,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev parallel,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev parport,id=id,path=path[,mux=on|off][,logfile=PATH][,logappend=on|off]
-chardev spicevmc,id=id,name=name[,debug=debug][,logfile=PATH][,logappend=on|off]
-chardev spiceport,id=id,name=name[,debug=debug][,logfile=PATH][,logappend=on|off]

Bluetooth(R) options:
-bt hci,null    dumb bluetooth HCI - doesn't respond to commands
-bt hci,host[:id]
                use host's HCI with the given name
-bt hci[,vlan=n]
                emulate a standard HCI in virtual scatternet 'n'
-bt vhci[,vlan=n]
                add host computer to virtual scatternet 'n' using VHCI
-bt device:dev[,vlan=n]
                emulate a bluetooth device 'dev' in scatternet 'n'

TPM device options:
-tpmdev passthrough,id=id[,path=path][,cancel-path=path]
                use path to provide path to a character device; default is /dev/tpm0
                use cancel-path to provide path to TPM's cancel sysfs entry; if
                not provided it will be searched for in /sys/class/misc/tpm?/device
-tpmdev emulator,id=id,chardev=dev
                configure the TPM device using chardev backend

Linux/Multiboot boot specific:
-kernel bzImage use 'bzImage' as kernel image
-append cmdline use 'cmdline' as kernel command line
-initrd file    use 'file' as initial ram disk
-dtb    file    use 'file' as device tree image

Debug/Expert options:
-fw_cfg [name=]<name>,file=<file>
                add named fw_cfg entry with contents from file
-fw_cfg [name=]<name>,string=<str>
                add named fw_cfg entry with contents from string
-serial dev     redirect the serial port to char device 'dev'
-parallel dev   redirect the parallel port to char device 'dev'
-monitor dev    redirect the monitor to char device 'dev'
-qmp dev        like -monitor but opens in 'control' mode
-qmp-pretty dev like -qmp but uses pretty JSON formatting
-mon [chardev=]name[,mode=readline|control][,pretty[=on|off]]
-debugcon dev   redirect the debug console to char device 'dev'
-pidfile file   write PID to 'file'
-singlestep     always run in singlestep mode
--preconfig     pause QEMU before machine is initialized (experimental)
-S              freeze CPU at startup (use 'c' to start execution)
-realtime [mlock=on|off]
                run qemu with realtime features
                mlock=on|off controls mlock support (default: on)
-overcommit [mem-lock=on|off][cpu-pm=on|off]
                run qemu with overcommit hints
                mem-lock=on|off controls memory lock support (default: off)
                cpu-pm=on|off controls cpu power management (default: off)
-gdb dev        wait for gdb connection on 'dev'
-s              shorthand for -gdb tcp::1234
-d item1,...    enable logging of specified items (use '-d help' for a list of log items)
-D logfile      output log to logfile (default stderr)
-dfilter range,..  filter debug output to range of addresses (useful for -d cpu,exec,etc..)
-L path         set the directory for the BIOS, VGA BIOS and keymaps
-bios file      set the filename for the BIOS
-enable-kvm     enable KVM full virtualization support
-enable-hax     enable HAX virtualization support
-xen-domid id   specify xen guest domain id
-xen-create     create domain using xen hypercalls, bypassing xend
                warning: should not be used when xend is in use
-xen-attach     attach to existing xen domain
                xend will use this when starting QEMU
-xen-domid-restrict     restrict set of available xen operations
                        to specified domain id. (Does not affect
                        xenpv machine type).
-no-reboot      exit instead of rebooting
-no-shutdown    stop before shutdown
-loadvm [tag|id]
                start right away with a saved state (loadvm in monitor)
-daemonize      daemonize QEMU after initializing
-option-rom rom load a file, rom, into the option ROM space
-rtc [base=utc|localtime|<datetime>][,clock=host|rt|vm][,driftfix=none|slew]
                set the RTC base and clock, enable drift fix for clock ticks (x86 only)
-icount [shift=N|auto][,align=on|off][,sleep=on|off,rr=record|replay,rrfile=<filename>,rrsnapshot=<snapshot>]
                enable virtual instruction counter with 2^N clock ticks per
                instruction, enable aligning the host and virtual clocks
                or disable real time cpu sleeping
-watchdog model
                enable virtual hardware watchdog [default=none]
-watchdog-action reset|shutdown|poweroff|inject-nmi|pause|debug|none
                action when watchdog fires [default=reset]
-echr chr       set terminal escape character instead of ctrl-a
-virtioconsole c
                set virtio console
-show-cursor    show cursor
-tb-size n      set TB size
-incoming tcp:[host]:port[,to=maxport][,ipv4][,ipv6]
-incoming rdma:host:port[,ipv4][,ipv6]
-incoming unix:socketpath
                prepare for incoming migration, listen on
                specified protocol and socket address
-incoming fd:fd
-incoming exec:cmdline
                accept incoming migration on given file descriptor
                or from given external command
-incoming defer
                wait for the URI to be specified via migrate_incoming
-only-migratable     allow only migratable devices
-nodefaults     don't create default devices
-chroot dir     chroot to dir just before starting the VM
-runas user     change to user id user just before starting the VM
                user can be numeric uid:gid instead
-sandbox on[,obsolete=allow|deny][,elevateprivileges=allow|deny|children]
          [,spawn=allow|deny][,resourcecontrol=allow|deny]
                Enable seccomp mode 2 system call filter (default 'off').
                use 'obsolete' to allow obsolete system calls that are provided
                    by the kernel, but typically no longer used by modern
                    C library implementations.
                use 'elevateprivileges' to allow or deny QEMU process to elevate
                    its privileges by blacklisting all set*uid|gid system calls.
                    The value 'children' will deny set*uid|gid system calls for
                    main QEMU process but will allow forks and execves to run unprivileged
                use 'spawn' to avoid QEMU to spawn new threads or processes by
                     blacklisting *fork and execve
                use 'resourcecontrol' to disable process affinity and schedular priority
-readconfig <file>
-writeconfig <file>
                read/write config file
-no-user-config
                do not load default user-provided config files at startup
-trace [[enable=]<pattern>][,events=<file>][,file=<file>]
                specify tracing options
-enable-fips    enable FIPS 140-2 compliance
-msg timestamp[=on|off]
                change the format of messages
                on|off controls leading timestamps (default:on)
-dump-vmstate <file>
                Output vmstate information in JSON format to file.
                Use the scripts/vmstate-static-checker.py file to
                check for possible regressions in migration code
                by comparing two such vmstate dumps.
-enable-sync-profile
                enable synchronization profiling

Generic object creation:
-object TYPENAME[,PROP1=VALUE1,...]
                create a new object of type TYPENAME setting properties
                in the order they are specified.  Note that the 'id'
                property must be set.  These objects are placed in the
                '/objects' path.

During emulation, the following keys are useful:
ctrl-alt-f      toggle full screen
ctrl-alt-n      switch to virtual console 'n'
ctrl-alt        toggle mouse and keyboard grab

When using -nographic, press 'ctrl-a h' to get some help.

See <https://qemu.org/contribute/report-a-bug> for how to report bugs.
More information on the QEMU project at <https://qemu.org>.)";
/* mockup

      aqemu start <VIRTUAL_MACHINE>
      aqemu pause <VIRTUAL_MACHINE>
      aqemu stop  <VIRTUAL_MACHINE>
      aqemu (-h | --help)
      aqemu --version

*/

void parse_qemu( int argc, char *argv[] );

