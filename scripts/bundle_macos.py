import os
import shutil

def info_plist():
    return '''
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
    <dict>
        <key>LSUIElement</key>
        <true/>
        <key>CFBundleGetInfoString</key>
        <string>Foo</string>
        <key>CFBundleExecutable</key>
        <string>ChollianWallpaper</string>
        <key>CFBundleIdentifier</key>
        <string>com.your-company-name.www</string>
        <key>CFBundleName</key>
        <string>Chollian Wallpaper</string>
        <key>CFBundleIconFile</key>
        <string>foo.icns</string>
        <key>CFBundleShortVersionString</key>
        <string>0.01</string>
        <key>CFBundleInfoDictionaryVersion</key>
        <string>6.0</string>
        <key>CFBundlePackageType</key>
        <string>APPL</string>
        <key>IFMajorVersion</key>
        <integer>0</integer>
        <key>IFMinorVersion</key>
        <integer>1</integer>
    </dict>
</plist>
    '''



if __name__ == "__main__":

    BUILD_DIR = "../build"
    APP_NAME = "Chollian Wallpaper.app"
    APP_NAME_IN_CMD = "Chollian\ Wallpaper.app"
    EXECUTABLE_NAME = "ChollianWallpaper"
    QT_FRAMEWORK_PATH = "/usr/local/Frameworks"

    os.chdir(BUILD_DIR)
    
    os.mkdir(APP_NAME)
    os.mkdir(os.path.join(APP_NAME, "Contents"))
    os.mkdir(os.path.join(APP_NAME, "Contents", "Frameworks"))
    os.mkdir(os.path.join(APP_NAME, "Contents", "MacOS"))
    os.mkdir(os.path.join(APP_NAME, "Contents", "Plugins"))
    os.mkdir(os.path.join(APP_NAME, "Contents", "Resources"))

    # Generate Info.plist
    f = open(os.path.join(APP_NAME, "Contents", "Info.plist"), 'w')
    f.write(info_plist())
    f.close()


    # Copy executable binary
    os.rename(EXECUTABLE_NAME, os.path.join(APP_NAME, "Contents", "MacOS", EXECUTABLE_NAME))

    # Copy qt dependency using macdeployqt
    os.system("macdeployqt " + APP_NAME_IN_CMD)
    shutil.copytree(os.path.join(QT_FRAMEWORK_PATH, "QtDBus.framework"), os.path.join(APP_NAME, "Contents", "Frameworks", "QtDBus.framework"), symlinks=True)


    # Fix dependency path contains @rpath manually

    QT_DBUS_FRAMEWORK_IN_BUNDLE = os.path.join(APP_NAME_IN_CMD, "Contents", "Frameworks", "QtDBus.framework", "QtDBus")
    QT_GUI_FRAMEWORK_IN_BUNDLE = os.path.join(APP_NAME_IN_CMD, "Contents", "Frameworks", "QtGui.framework", "QtGui")
    QT_WIDGETS_FRAMEWORK_IN_BUNDLE = os.path.join(APP_NAME_IN_CMD, "Contents", "Frameworks", "QtWidgets.framework", "QtWidgets")

    os.system("install_name_tool -change @rpath/QtCore.framework/Versions/A/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/A/QtCore " + QT_DBUS_FRAMEWORK_IN_BUNDLE)

    os.system("install_name_tool -change @rpath/QtDBus.framework/Versions/A/QtDBus @executable_path/../Frameworks/QtDBus.framework/Versions/A/QtDBus " + QT_GUI_FRAMEWORK_IN_BUNDLE)
    os.system("install_name_tool -change @rpath/QtCore.framework/Versions/A/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/A/QtCore " + QT_GUI_FRAMEWORK_IN_BUNDLE)

    os.system("install_name_tool -change @rpath/QtGui.framework/Versions/A/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/A/QtGui " + QT_WIDGETS_FRAMEWORK_IN_BUNDLE)
    os.system("install_name_tool -change @rpath/QtCore.framework/Versions/A/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/A/QtCore " + QT_WIDGETS_FRAMEWORK_IN_BUNDLE)

    # Copy resources (icon)
    shutil.copy(os.path.join("..", "resources", "icon.png"), os.path.join(APP_NAME, "Contents", "Resources", "icon.png"))


    print("**********************************************")
    print("*                                            *")
    print("*  Ignore ERROR message, packaging is done.  *")
    print("*                                            *")
    print("**********************************************")