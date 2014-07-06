creat_wpa_supplicant_conf()
{
    ssid = ${nvram show wl0_apcli_rule 0 ssid}
    secmode = ${nvram show wl0_apcli_rule 0 secmode}
    wpa_crypto = ${nvram show wl0_wpa_auth_rule 0 wpa_crypto}
    wpa_auth = ${nvram show wl0_wpa_auth_rule 0 wpa_auth}
    wpa_user = ${nvram show wl0_wpa_auth_rule 0 wpa_user}
    wpa_passwd = ${nvram show wl0_wpa_auth_rule 0 wpa_passwd}



   if[ "$secmode" == "wpa" ] ;then
     proto = 'WPA'
   else
     proto = 'RSN'
   fi
   
   if[ "$wpa_crypto" == "1" ] ;then
     pairwise = 'CCMP'
     group = 'CCMP'
   else
     pairwise = 'TKIP'
     group = 'TKIP'
   fi
   
   if[ "wpa_auth" == "1" ] ;then
     eap = 'PEAP'
   else
     eap = 'TTLS'
   fi
   
    cat > /var/run/wpa_supplicant.conf <<EOF
network={
        ssid="$ssid"
        scan_ssid=1
        key_mgmt=WPA_EAP
        proto=$proto
        pairwise="$pairwise"
        group="$group"
        eap="$eap"
	identity="$wpa_user"
	password="$wpa_passwd"
	phase2="auth=MSCHAPV2"
       
}
EOF

                    
}
