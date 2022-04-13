关于熔断幽灵漏洞

对于window

win+r     regedit

点开HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management
设置键值　

　"FeatureSettingsOverrideMask"=dword:00000003
　　"FeatureSettingsOverride"=dword:00000003



对于ubuntu

```
vi /etc/default/grub
```

```
修改为GRUB_CMDLINE_LINUX_DEFAULT="  .....  pti=off    spectre_v2=off  "
```

```
update-grub
```

reboot

cat /proc/cmdline查看是否有pti=off  spectre_v2=off 

