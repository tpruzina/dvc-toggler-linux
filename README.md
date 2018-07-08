# DVC toggler for Linux

Small QT5 app that controls digital vibrance (a.k.a saturation) on per application
profile basis. Inspired by windows DVC togglers.

## Status

Very much WIP, but basic functionality should work.
See [TODO](TODO) for features that are yet to be implemented.
No binary releases at this time.


![Imgur](https://i.imgur.com/pp84mIP.png)

## Build Requirements
1. C++17 compiler
2. QT5
3. Meson
4. pkg-config (pkgconf)
4. Xorg headers (x11, xext, xmu)
5. dbus-1


```
sudo apt install build-essential qt5-default libqt5svg5-dev libx11-dev libxext-dev libxmu-dev pkgconf libdbus-1-dev git meson
```

## Build Instructions
```
git clone https://gitlab.com/pruzinat/dvc-toggler-linux.git
cd dvc-toggler-linux
meson build
cd build
ninja
sudo ninja install
```

## Notes

By default, user configuration is stored in ```$HOME/.config/dvc-toggler/settings.ini```

## License

Modified BSD, see [COPYING](COPYING).

## Bugs

This is barely tested, any and all bug reports are more than wellcome.

## Notes II

Originally this was a much broader project, WIP title "gamePref", but due to lack of time and ultimately intrest
I have scrapped the original project in 2015. This is a frankenstein monster created on dead bones of gamePref,
code quality reflects that although I put _some_ effort towards cleaning this thing up into source code presentable state.
Have those pesky NVIDIA bits not have been GPLv2 licensed, I would have released this under [GLWTPL](https://github.com/me-shaon/GLWTPL/blob/master/LICENSE).
