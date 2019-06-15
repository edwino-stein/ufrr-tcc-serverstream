# ServerStream

Servidor de stream de vídeo em MPEG-1/MPEG-TS via WebSocket.

### Intalação dos scripts para serviço linux

Configure os parâmetros necessários nos arquivos `deploy/ffmpegd.sh` e  `deploy/serverstreamd.sh`. Então execute:

```
$ sudo cp deploy/ffmpegd.sh /etc/init.d/ffmpegd
$ sudo cp deploy/serverstreamd.sh /etc/init.d/serverstreamd
$ sudo chmod +x /etc/init.d/ffmpegd /etc/init.d/serverstreamd
$ sudo chmod +x deploy/run-ffmpeg.sh deploy/run-serverstream.sh
$ sudo update-rc.d ffmpegd defaults
$ sudo update-rc.d serverstreamd defaults
```

### Remoção dos scritps para serviço

```
$ sudo service ffmpegd stop
$ sudo update-rc.d ffmpegd remove
$ sudo service serverstreamd stop
$ sudo update-rc.d serverstreamd remove
$ sudo rm /etc/init.d/ffmpegd /etc/init.d/serverstreamd
```
