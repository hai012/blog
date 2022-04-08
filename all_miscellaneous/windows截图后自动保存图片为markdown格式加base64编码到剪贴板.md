# 1、安装需要的依赖模块

```
pip install pyperclip
pip install pillow
```

如果没有网络可通过其他平台下载文件本地安装，有两种方式：
一、通过tar包安装

1.https://pypi.org/，到这个网站下载所需要安装的模块文件，在侧边栏Download files可以找到模块安装文件。
2.解压下载的安装包，进入包含setup.py文件的目录，在cmd中执行命令：python setup.py install
注意，要把下载连接中的hash值去掉，即
https://files.pythonhosted.org/packages/94/a3/3dbc737fc3ab5f51ae49344fdd6183202659c692504cfaf5e40a08d50b4b/pyperclip-1.5.0.zip#sha256=c6fe742dc64a4e17ba97d8bdc7ea89fc94b40dd26954a2f0a12db38d6f0c75be
要变成
https://files.pythonhosted.org/packages/94/a3/3dbc737fc3ab5f51ae49344fdd6183202659c692504cfaf5e40a08d50b4b/pyperclip-1.5.0.zip
才能用浏览器下载

二、通过pip+whl
	去 https://www.lfd.uci.edu/~gohlke/pythonlibs/  下载whl包，然后
    pip install  xxx.whl

# 2、后台运行
```
python cliper2base64.py
```

cliper2base64.py内容如下：
```
import sys
import time
import base64
from PIL import Image
from PIL import ImageGrab
import pyperclip
from io import BytesIO

if __name__ == "__main__":
    while True:
        while True:
            time.sleep(1)
            image = ImageGrab.grabclipboard()
            if isinstance(image, Image.Image):
                break
        img_buffer = BytesIO()
        image.save(img_buffer, format='png', optimize=True)
        byte_data = img_buffer.getvalue()
        objBase64 = base64.b64encode(byte_data)
        strBase64 = (str(objBase64))[2:-1]
        strHash = str(hash(str))
        msg = '![][' + strHash + ']\n\n[' + strHash +']:data:image/' + 'png;base64,' + strBase64
        pyperclip.copy(msg)
    sys.exit(0)
```
