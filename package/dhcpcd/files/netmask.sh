#!/bin/sh

prefix2mask()
{
	if [ $# -ne 1 ]; then
		echo "usage: prefix2mask() <prefix>"
		return 1
	fi

	case "$1" in
		32)
			echo '255.255.255.255'		
			;;
		31)
			echo '255.255.255.254'		
			;;
		30)
			echo '255.255.255.252'		
			;;
		29)
			echo '255.255.255.248'		
			;;
		28)
			echo '255.255.255.240'		
			;;
		27)
			echo '255.255.255.224'		
			;;
		26)
			echo '255.255.255.192'		
			;;
		25)
			echo '255.255.255.128'		
			;;

		24)
			echo '255.255.255.0'		
			;;
		23)
			echo '255.255.254.0'		
			;;
		22)
			echo '255.255.252.0'		
			;;
		21)
			echo '255.255.248.0'		
			;;
		20)
			echo '255.255.240.0'		
			;;
		19)
			echo '255.255.224.0'		
			;;
		18)
			echo '255.255.192.0'		
			;;
		17)
			echo '255.255.128.0'		
			;;

		16)
			echo '255.255.0.0'		
			;;
		15)
			echo '255.254.0.0'		
			;;
		14)
			echo '255.252.0.0'		
			;;
		13)
			echo '255.248.0.0'		
			;;
		12)
			echo '255.240.0.0'		
			;;
		11)
			echo '255.224.0.0'		
			;;
		10)
			echo '255.192.0.0'		
			;;
		9)
			echo '255.128.0.0'		
			;;

		8)
			echo '255.0.0.0'		
			;;
		7)
			echo '254.0.0.0'		
			;;
		6)
			echo '252.0.0.0'		
			;;
		5)
			echo '248.0.0.0'		
			;;
		4)
			echo '240.0.0.0'		
			;;
		3)
			echo '224.0.0.0'		
			;;
		2)
			echo '192.0.0.0'		
			;;
		1)
			echo '128.0.0.0'		
			;;
		*)
			return 1
			;;
	esac

	return 0
}

mask2prefix()
{
	if [ $# -ne 1 ]; then
		echo "usage: mask2prefix() <mask>"
		return 1
	fi

	if [ "$1" = "255.255.255.255"		 ]; then
		echo '32'		;return 0;
	fi
	if [ "$1" = "255.255.255.254"		 ]; then
		echo '31'		;return 0;
	fi
	if [ "$1" = "255.255.255.252"		 ]; then
		echo '30'		;return 0;
	fi
	if [ "$1" = "255.255.255.248"		 ]; then
		echo '29'		;return 0;
	fi
	if [ "$1" = "255.255.255.240"		 ]; then
		echo '28'		;return 0;
	fi
	if [ "$1" = "255.255.255.224"		 ]; then
		echo '27'		;return 0;
	fi
	if [ "$1" = "255.255.255.192"		 ]; then
		echo '26'		;return 0;
	fi
	if [ "$1" = "255.255.255.128"		 ]; then
		echo '25'		;return 0;
	fi
	if [ "$1" = "255.255.255.0"		 ]; then
		echo '24'		;return 0;
	fi
	if [ "$1" = "255.255.254.0"		 ]; then
		echo '23'		;return 0;
	fi
	if [ "$1" = "255.255.252.0"		 ]; then
		echo '22'		;return 0;
	fi
	if [ "$1" = "255.255.248.0"		 ]; then
		echo '21'		;return 0;
	fi
	if [ "$1" = "255.255.240.0"		 ]; then
		echo '20'		;return 0;
	fi
	if [ "$1" = "255.255.224.0"		 ]; then
		echo '19'		;return 0;
	fi
	if [ "$1" = "255.255.192.0"		 ]; then
		echo '18'		;return 0;
	fi
	if [ "$1" = "255.255.128.0"		 ]; then
		echo '17'		;return 0;
	fi
	if [ "$1" = "255.255.0.0"		 ]; then
		echo '16'		;return 0;
	fi
	if [ "$1" = "255.254.0.0"		 ]; then
		echo '15'		;return 0;
	fi
	if [ "$1" = "255.252.0.0"		 ]; then
		echo '14'		;return 0;
	fi
	if [ "$1" = "255.248.0.0"		 ]; then
		echo '13'		;return 0;
	fi
	if [ "$1" = "255.240.0.0"		 ]; then
		echo '12'		;return 0;
	fi
	if [ "$1" = "255.224.0.0"		 ]; then
		echo '11'		;return 0;
	fi
	if [ "$1" = "255.192.0.0"		 ]; then
		echo '10'		;return 0;
	fi
	if [ "$1" = "255.128.0.0"		 ]; then
		echo '9'		;return 0;
	fi
	if [ "$1" = "255.0.0.0"		 ]; then
		echo '8'		;return 0;
	fi
	if [ "$1" = "254.0.0.0"		 ]; then
		echo '7'		;return 0;
	fi
	if [ "$1" = "252.0.0.0"		 ]; then
		echo '6'		;return 0;
	fi
	if [ "$1" = "248.0.0.0"		 ]; then
		echo '5'		;return 0;
	fi
	if [ "$1" = "240.0.0.0"		 ]; then
		echo '4'		;return 0;
	fi
	if [ "$1" = "224.0.0.0"		 ]; then
		echo '3'		;return 0;
	fi
	if [ "$1" = "192.0.0.0"		 ]; then
		echo '2'		;return 0;
	fi
	if [ "$1" = "128.0.0.0"		 ]; then
		echo '1'		;return 0;
	fi

	return 1
}
