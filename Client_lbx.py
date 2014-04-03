# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'QClient.ui'
#
# Created: Wed Mar 19 08:47:22 2014
#      by: PyQt4 UI code generator 4.10.3
#
# WARNING! All changes made in this file will be lost!

import sys,json,time,socket
sys.path.append('/usr/local/lib/python2.7/site-packages')

from PyQt4 import QtCore, QtGui
try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

flag = False

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName(_fromUtf8("Dialog"))
        Dialog.resize(400, 522)
        self.IPlabel = QtGui.QLabel(Dialog)
        self.IPlabel.setGeometry(QtCore.QRect(20, 10, 31, 17))
        self.IPlabel.setObjectName(_fromUtf8("IPlabel"))
        self.IPEdit = QtGui.QLineEdit(Dialog)
        self.IPEdit.setGeometry(QtCore.QRect(20, 30, 181, 27))
        self.IPEdit.setObjectName(_fromUtf8("IPEdit"))
        self.PortEdit = QtGui.QLineEdit(Dialog)
        self.PortEdit.setGeometry(QtCore.QRect(210, 30, 51, 27))
        self.PortEdit.setObjectName(_fromUtf8("PortEdit"))
        self.portlabel = QtGui.QLabel(Dialog)
        self.portlabel.setGeometry(QtCore.QRect(210, 10, 66, 17))
        self.portlabel.setObjectName(_fromUtf8("portlabel"))
        self.connectButton = QtGui.QPushButton(Dialog)
        self.connectButton.setGeometry(QtCore.QRect(290, 30, 98, 27))
        self.connectButton.setObjectName(_fromUtf8("connectButton"))
        self.filePathEdit = QtGui.QLineEdit(Dialog)
        self.filePathEdit.setGeometry(QtCore.QRect(20, 130, 241, 27))
        self.filePathEdit.setObjectName(_fromUtf8("filePathEdit"))
        self.filelabel = QtGui.QLabel(Dialog)
        self.filelabel.setGeometry(QtCore.QRect(20, 110, 66, 17))
        self.filelabel.setObjectName(_fromUtf8("filelabel"))
        self.sendBrowserButton = QtGui.QPushButton(Dialog)
        self.sendBrowserButton.setGeometry(QtCore.QRect(290, 130, 98, 27))
        self.sendBrowserButton.setObjectName(_fromUtf8("sendBrowserButton"))
        self.sendButton = QtGui.QPushButton(Dialog)
        self.sendButton.setGeometry(QtCore.QRect(290, 170, 98, 27))
        self.sendButton.setObjectName(_fromUtf8("sendButton"))
        self.receiveFilePath = QtGui.QLineEdit(Dialog)
        self.receiveFilePath.setGeometry(QtCore.QRect(30, 440, 241, 27))
        self.receiveFilePath.setObjectName(_fromUtf8("receiveFilePath"))
        self.receiveButton = QtGui.QPushButton(Dialog)
        self.receiveButton.setGeometry(QtCore.QRect(290, 440, 98, 27))
        self.receiveButton.setObjectName(_fromUtf8("receiveButton"))
        self.FileNameEdit = QtGui.QLineEdit(Dialog)
        self.FileNameEdit.setGeometry(QtCore.QRect(30, 390, 241, 27))
        self.FileNameEdit.setObjectName(_fromUtf8("lineEdit"))
        self.FileNamelabel = QtGui.QLabel(Dialog)
        self.FileNamelabel.setGeometry(QtCore.QRect(30, 370, 81, 17))
        self.FileNamelabel.setObjectName(_fromUtf8("FileNamelabel"))
        self.FilePathlabel = QtGui.QLabel(Dialog)
        self.FilePathlabel.setGeometry(QtCore.QRect(30, 420, 111, 17))
        self.FilePathlabel.setObjectName(_fromUtf8("FilePathlabel"))

        self.textEdit = QtGui.QTextEdit(Dialog)
        self.textEdit.setGeometry(QtCore.QRect(60, 210, 291, 151))
        self.textEdit.setObjectName(_fromUtf8("textEdit"))

        self.connectButton.clicked.connect(self.connection)
        self.sendButton.clicked.connect(self.sendFile)
        self.receiveButton.clicked.connect(self.receiveFile)
        self.sendBrowserButton.clicked.connect(self.selectFile)

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(_translate("Dialog", "Dialog", None))
        self.IPlabel.setText(_translate("Dialog", "IP：", None))
        self.portlabel.setText(_translate("Dialog", "Port：", None))
        self.connectButton.setText(_translate("Dialog", "连接", None))
        self.filelabel.setText(_translate("Dialog", "File:", None))
        self.sendBrowserButton.setText(_translate("Dialog", "浏览", None))
        self.sendButton.setText(_translate("Dialog", "发送", None))
        # self.scrollArea.setToolTip(_translate("Dialog", "<html><head/><body><p><br/></p></body></html>", None))
        self.receiveButton.setText(_translate("Dialog", "提取", None))
        self.FileNamelabel.setText(_translate("Dialog", "FileName:", None))
        self.FilePathlabel.setText(_translate("Dialog", "FilePath:", None))

    def selectFile(self, Dialog):
        self.filePathEdit.setText(QtGui.QFileDialog.getOpenFileName())

    def listFiles(self):
        r = s.recv(4096)
        print r
        rdata = json.loads(r)
        self.textEdit.clear()
        self.textEdit.append("Files in server:")
        for x in rdata:
            self.textEdit.append(x)

    def connection(self, Dialog):
        global flag
        if not flag:
            try:
                ip = str(self.IPEdit.text())
                port = int(self.PortEdit.text())
                s.connect((ip,port))
                QtGui.QMessageBox.about(self.connectButton,"success","connection is sucess!")
                
                self.listFiles()
                
                flag = True
            except Exception, e:
                QtGui.QMessageBox.critical(self.connectButton,"error","failed! Please check your ip and port")
        else:
            QtGui.QMessageBox.about(self.connectButton,"success","already connected")

    def confirm(self, s, client_command):
        s.send('\t'.join(client_command))
        data = s.recv(4096)
        if data == 'ready':
            return True

    def receiveFile(self, Dialog):
        ReceiveFileName = self.FileNameEdit.text()
        fileType = ReceiveFileName.split('.')[-1]
        action = 'get'
        self.receiveFilePath.setText(QtGui.QFileDialog.getExistingDirectory())
        filename =  self.receiveFilePath.text()+ '/'  + ReceiveFileName
        filename = str(filename)
        client_command = (action, str(ReceiveFileName))
        print client_command
        if self.confirm(s, client_command):
            f = open(filename, 'wb')
            while True:
                data = s.recv(4096)
                if data == 'EOF':
                    print "recv file success!"
                    break
                f.write(data)
            f.close()
            QtGui.QMessageBox.about(self.receiveButton,"receive success","receiving is success!")
            self.receiveFilePath.clear()
            self.FileNameEdit.clear()
            if(fileType == 'jpg' or fileType == 'png' or fileType == 'bmp'):
                msg = QtGui.QMessageBox() 
                msg.setIconPixmap(QtGui.QPixmap(filename))
                msg.exec_()
        else:
            QtGui.QMessageBox.critical(self.receiveButton,"receiving failed","receiving is failed!")


    def sendFile(self, Dialog):
        theSendFile = open(self.filePathEdit.text(), 'rb')
        action = 'put' 
        filename = str(theSendFile.name)
        client_command = (action, filename)
        print client_command
        if self.confirm(s, client_command):    
            while True:
                data = theSendFile.read(4096)
                if not data:
                    break
                s.sendall(data)
            theSendFile.close()
            time.sleep(1)
            s.sendall('EOF')
            QtGui.QMessageBox.about(self.sendButton,"sending over","sending over")
            self.listFiles()
        else:
            QtGui.QMessageBox.critical(self.sendButton,"sending failed","sending failed")
        


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    Dialog = QtGui.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.show()
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sys.exit(app.exec_())

