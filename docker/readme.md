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
### local docker container
```sh
xhost +local:
sudo docker run \
  --rm \
  -p 8080:8080 \
  -ti \
  --env="DISPLAY=$DISPLAY" \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
  --volume="$(pwd):/opt/workdir" \
  --workdir="/opt/workdir" \
  vscode \
    /usr/bin/code --verbose ./
```

### over ssh x11 forwarding

* https://stackoverflow.com/questions/38249629/inside-a-docker-container-error-cannot-open-display-localhost11-0
* https://unix.stackexchange.com/questions/110558/su-with-error-x11-connection-rejected-because-of-wrong-authentication
* `--volume /home/` __ubuntu__ `/.Xauthority:/home/` __vscode__ `/.Xauthority:rw \` <- check it
```
ssh -XC foo@bar
```

```
sudo docker run \
  --privileged \
  --rm \
  -ti \
  --env DISPLAY=$DISPLAY \
  --net host \
  --volume /tmp/.X11-unix:/tmp/.X11-unix:rw \
  --volume /home/ubuntu/.Xauthority:/home/vscode/.Xauthority:rw \
  --volume $(pwd):/opt/workdir \
  --workdir /opt/workdir \
  vscode \
    /usr/bin/code --verbose ./
```


### novnc

```sh
sudo /usr/bin/supervisord -c /etc/supervisor/supervisord.conf
```
