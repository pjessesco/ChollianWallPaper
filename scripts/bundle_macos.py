import os
import shutil
import argparse


def info_plist(version):
    return '''
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
    <dict>
        <key>LSUIElement</key>
        <true/>
        <key>CFBundleExecutable</key>
        <string>ChollianWallpaper</string>
        <key>CFBundleIdentifier</key>
        <string>com.pjessesco.ChollianWallpaper</string>
        <key>CFBundleName</key>
        <string>Chollian Wallpaper</string>
        <key>CFBundleVersion</key>
        <string>''' + version + '''</string>
        <key>NSHumanReadableCopyright</key>
        <string>© 2021 Jino Park</string>
        <key>CFBundleGetInfoString</key>
        <string>Wallpaper changer using near-realtime satellite images(Chollian 2A from South Korea)</string>
        <key>CFBundleIconFile</key>
        <string>foo.icns</string>
        <key>CFBundleInfoDictionaryVersion</key>
        <string>6.0</string>
        <key>CFBundlePackageType</key>
        <string>APPL</string>
    </dict>
</plist>
    '''



if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("--qt_dir", type=str, required=True)
    parser.add_argument("--build_dir", type=str, required=True)
    parser.add_argument("--version", type=str, required=True)

    args = parser.parse_args()

    BUILD_DIR = args.build_dir
    QT_PATH = args.qt_dir
    CHOLLIAN_VERSION_STR = args.version


    APP_NAME = "Chollian Wallpaper.app"
    APP_NAME_IN_CMD = "Chollian\ Wallpaper.app"
    EXECUTABLE_NAME = "ChollianWallpaper"

    os.chdir(BUILD_DIR)
    
    os.makedirs(APP_NAME)
    os.mkdir(os.path.join(APP_NAME, "Contents"))
    os.mkdir(os.path.join(APP_NAME, "Contents", "Frameworks"))
    os.mkdir(os.path.join(APP_NAME, "Contents", "MacOS"))
    os.mkdir(os.path.join(APP_NAME, "Contents", "Plugins"))
    os.mkdir(os.path.join(APP_NAME, "Contents", "Resources"))

    # Generate Info.plist
    f = open(os.path.join(APP_NAME, "Contents", "Info.plist"), 'w')
    f.write(info_plist(CHOLLIAN_VERSION_STR))
    f.close()

    # Copy executable binary
    os.rename(EXECUTABLE_NAME, os.path.join(APP_NAME, "Contents", "MacOS", EXECUTABLE_NAME))

     # Copy resources (icon)
    shutil.copy(os.path.join("..", "resources", "icon.png"), os.path.join(APP_NAME, "Contents", "Resources", "icon.png"))

    # Copy qt dependency using macdeployqt
    os.system(QT_PATH+"bin/macdeployqt " + APP_NAME_IN_CMD)

    # Generate DMG
    os.mkdir("bundle")
    shutil.move(APP_NAME, "bundle/"+APP_NAME)
    os.system("create-dmg --no-internet-enable --app-drop-link 0 0 \"Chollian Wallpaper.dmg\" \"bundle\"")
    

    