#!/bin/sh 
HELP_STR="sudo $0 <adaptor> <restart_service> [adaptor_name], ex: sudo $0 hci0 1 or sudo $0 hci0 BuildServerTest-linstPC"

function get_bluez_version()
{
  BLUEZ_VERSION=$(rpm -qa | grep bluez-[0-9].[0-9])
  echo 
  echo "BLUEZ_VERSION = $BLUEZ_VERSION"
  echo "$BLUEZ_VERSION" > tmp.$$
  #grep "bluez-4." tmp.$$
  echo
}

function sleep_fun()
{
  echo "sleep $1..."
  sleep $1
}

function service_restart()
{
  if [ "$1" -eq 1 ]
  then
    printf "bluetooth.service status\n"
    systemctl list-units | grep bluetooth
    echo

    printf "Stop bluetooth.service!\n"
    systemctl stop bluetooth.service
    if [ "$?" -ne 0 ]
    then
      printf "Stop bluetooth service error! exit! \n"
      exit 1
    fi
    sleep_fun 1s
    echo

    printf "bluetooth.service status\n"
    systemctl list-units | grep bluetooth
    echo

    printf "Start bluetooth.service!\n"
    systemctl restart bluetooth.service
    if [ "$?" -ne 0 ]
    then
      printf "Start bluetooth service error! exit! \n"
      exit 1
    fi
    sleep_fun 1s
    echo

    printf "bluetooth.service status\n"
    systemctl list-units | grep bluetooth
    echo
  else
    printf "Keep bluetooth.service status!\n"
  fi
}

function check_error()
{
  if [ "$?" -ne 0 ]
  then
    printf "check_error! exit! \n"
    exit 1
  fi
}

function check_input_parameter_empty_error()
{
  if [ -z "$1" ]
  then
    printf "Error! $HELP_STR\n"
    exit 1
  fi
}

function check_adaptor_input_error()
{
  if [ "$?" -ne 0 ]
  then
    printf "check_adaptor_input_error! $HELP_STR\n"
    exit 1
  fi
}

printf "Script status\n"
echo "\$*=\"$*\""
printf "\$1=\"%s\"\n" $1

get_bluez_version;
grep "bluez-[4-5]." tmp.$$
if [ "$?" -ne 0 ]
then
  printf "This script support only bluez-4.x and bluez-5.x! exit! \n"
  rm tmp.$$
  exit 1
fi
rm tmp.$$

check_input_parameter_empty_error $1;
check_input_parameter_empty_error $2;

echo "$1" | grep -q "hci[0-9]"
check_adaptor_input_error;

printf "BT service:\n"
service_restart $2;

printf "BT adapter setting start.\n"
printf "BT adapter status\n"
hciconfig

printf "BT adapter reset\n"
hciconfig $1 reset
check_error;
hciconfig
sleep_fun 1s

printf "BT adapter enable discoverable/auth/encrypt\n"
hciconfig $1 piscan auth encrypt 
check_error;
sleep_fun 1s
hciconfig
if [ $# -ge 3 ]
then
  printf "BT adapter set name\n"
  echo "original name:"
  hciconfig $1 name | grep -i "name:"
  check_error;
  echo "set new name:"
  hciconfig $1 name $3 
  sleep_fun 1s
  hciconfig $1 name | grep -i "name:"
  check_error;
  sleep_fun 1s
else
  echo "keep default name:"
  hciconfig $1 name | grep -i "name:"
  check_error;
fi

printf "BT adapter setting result:\n"
hciconfig -a
