* https://hub.docker.com/r/uphy/eclipse-vscode-jp/~/dockerfile/
* https://hub.docker.com/r/uphy/ubuntu-desktop-jp/~/dockerfile/

## build

```sh
sudo docker build \
  --build-arg user_id=$UID \
  --build-arg group_id=$GID \
  -t vscode \
  ./
```

## run

```sh
xhost +local: && sudo docker run \
  --rm \
  -p 8080:8080 \
  -ti \
  --env="DISPLAY=$DISPLAY" \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
  --volume="$(pwd):/opt/workdir" \
  --workdir="/opt/workdir" \
  vscode
```

### vscode

```sh
/usr/bin/code --verbose ./
```

### novnc

```sh
sudo /usr/bin/supervisord -c /etc/supervisor/supervisord.conf
```
