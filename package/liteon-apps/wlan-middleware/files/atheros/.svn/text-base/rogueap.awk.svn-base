/Cell/{
	Cell=""; Address="";
	ESSID="";
	Mode=0;
	Channel=0;
	Level=0;
	EnKey=0;
	WMode="";
	WPA2=0; WPA=0;
	AuthMode=0;
	wpa2crypto_ccmp=0; wpa2crypto_tkip=0; wpa2crypto=0;
	wpacrypto_ccmp=0; wpacrypto_tkip=0; wpacrypto=0;
	crypto=0;firstEncryption=1;
	auth=1; firstauth=1;
	Cell=$2; Address=$5;
}
/ESSID:/{
	ESSID=substr($0, 27);}
/Mode:/{
	if (match($0, "Auto")) {
		Mode=0;
	}
	else if(match($0, "Ad-Hoc")) {
		Mode=1;
	}
	else if(match($0, "Managed")) {
		Mode=2;
	}
	else if(match($0, "Repeater")) {
		Mode=3;
	}
	else if(match($0, "Master")) {
		Mode=4;
	}
	else if(match($0, "Secondary")) {
		Mode=5;
	}
	else if(match($0, "Monitor")) {
		Mode=6;
	}
	else {
		Mode=7;
	}
}
/Channel/{Channel=substr($4, 0, length($4)-1);}
/Signal/{Level=substr($3,8);}
/Encryption/{
	if(match($0,"off")) {
		EnKey=0;
	} 
	else {
		EnKey=1;
	}
}
/Protocol:/{
	WMode=substr($1,10);
	if((0==length(WMode)) || (match(WMode," "))) {
		WMode="Unknown"
	}
}
/WPA2/{WPA2=1;}
/IE:\ WPA/{WPA=1;}
/Pairwise\ Ciphers/{
	if((1 == firstEncryption) && (1==WPA2)) {
		if( match($0, "TKIP")) {
			wpa2crypto_tkip=2;
		}
		if( match($0, "CCMP")) {
			wpa2crypto_ccmp=1;
		}
		wpa2crypto=wpa2crypto_tkip+wpa2crypto_ccmp;
		firstEncryption=0;
	}
	else {
		if( match($0, "TKIP")) {
			wpacrypto_tkip=2;
		}
		if( match($0, "CCMP")) {
			wpacrypto_ccmp=1;
		}
		wpacrypto=wpacrypto_ccmp+wpacrypto_tkip;
	}
}
/Authentication\ Suites/{
	if(1 == firstauth) {
		if(match($0, "PSK")) {
			auth = 1;
		}
		else {
			auth = 2;
		}
		firstauth=0;
	}
}
/^$/{
	if(1==EnKey) {
		if(WPA2==1 && WPA ==1) {
			if(1==auth) {
				# WPAPSKWPA2PSK
				AuthMode=3;
			}
			else {
				# WPAWPA2
				AuthMode=6;
			}

			if((0==wpa2crypto) || (0==wpacrypto)) {
				crypto=50;
			}
			else if(wpa2crypto==wpacrypto) {
				crypto=wpa2crypto;
			}
			else {
				crypto=wpa2crypto*10+wpacrypto;
			}
		}
		else if(WPA2==1 && WPA ==0) {
			if(1==auth) {   
				# WPA2PSK
				AuthMode=2;  
			}            
			else {                                      
				# WPA2
				AuthMode=5;                                 
			}                                                       

			if(0==wpa2crypto) {
				crypto=50;
			}
			else {
				crypto=wpa2crypto;
			}
		}
		else if(WPA2==0 && WPA ==1) {
			if(1==auth) {
				# WPAPSK
				AuthMode=1;
			}
			else {
				# WPA
				AuthMode=4;
			}

			if(0==wpacrypto) {
				crypto=50;
			}
			else {
				crypto=wpacrypto;
			}
		}
		else {
			# WEP
			AuthMode=7;
			crypto=4;
		}
	}
	else {
		AuthMode=0;
		crypto=0;
	}
	if(NR!=2) {
		print Cell "\t" Address "\t" Mode "\t" Channel "\t" Level "\t" WMode "\t" crypto "\t" AuthMode "\t" ESSID 
	}
}
