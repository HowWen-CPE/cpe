pre_host_module.c
    Preip layer 2 control process at kernel space. 
    Receive command from user space via netlink, then send it out by dev_queue_xmit.
    Receive LLC packet from device, then deliver it via netlink to user space for parsing.

preip_host.c
    preip layer 2 control process at user space.
