# Listing ports:
# python -m serial.tools.list_ports

import csv, copy, os
import serial
from tkinter import *
from tkinter import filedialog

class Window(Frame):

    def __init__(self, parent):
        Frame.__init__(self, parent)
        self.parent = parent
        self.initUI()
        self.hasLoadedFile = False

    def initUI(self):

        self.parent.title("A/B Machines DVI Matrix Control")
        self.pack(fill=BOTH, expand=1)

        self.currentCue = 0
        self.cueListLength = 0

        menuBar = Menu(self.parent)
        self.parent.config(menu=menuBar)

        fileMenu = Menu(menuBar)
        fileMenu.add_command(label='Load', command=self.onOpen)
        menuBar.add_cascade(label='File', menu=fileMenu)

        self.pCueText = StringVar()
        self.pCueText.set('Previous Cue:')
        self.cCueText = StringVar()
        self.cCueText.set('Current Cue:')
        self.nCueText = StringVar()
        self.nCueText.set('Next Cue:')

        pCueLabel = Label(self, textvariable=self.pCueText, justify=LEFT)
        pCueLabel.place(x=35, y=20)
        cCueLabel = Label(self, textvariable=self.cCueText, justify=LEFT)
        cCueLabel.place(x=35, y=40)
        nCueLabel = Label(self, textvariable=self.nCueText, justify=LEFT)
        nCueLabel.place(x=35, y=60)

        previousButton = Button(self, text='<', justify=CENTER, command=self.previousCue)
        previousButton.place(x=50, y=100)

        nextButton = Button(self, text='>', justify=CENTER, command=self.nextCue)
        nextButton.place(x=100, y=100)

        quitButton = Button(self, text='Goto', justify=CENTER)
        quitButton.place(x=175, y=100)

        gotoInput = Entry(self)
        gotoInput.place(x=235, y=100, width=125)

    def onOpen(self):
        ft = [('CSV files', '*.csv'), ('All files', '*')]
        dlg = filedialog.Open(self, filetypes=ft)
        fl = dlg.show()
        self.readFile(fl)
        return 112

    def readFile(self, filename):
        with open(filename) as csvfile:
            self.cueList = list(csv.reader(csvfile, delimiter=','))[1:]
            self.cueListLength = len(self.cueList)
        if self.cueListLength > 0:
            self.hasLoadedFile = True
            self.currentCue = 0
            self.updateCueText()
        csvfile.closed
        return 112

    def updateCueText(self):
        if self.hasLoadedFile:
            # Previous Cue
            if self.currentCue == 0: self.pCueText.set('Previous Cue:\tNone')
            else: self.pCueText.set('Previous Cue:\t' + str(self.cueList[self.currentCue-1][0]))
            # Current Cue
            t = 'Current Cue:\t' + str(self.cueList[self.currentCue][0])
            self.cCueText.set(t)
            # Next Cue
            if self.currentCue == self.cueListLength-1: self.nCueText.set('Next Cue:\t\tNone')
            else: self.nCueText.set('Next Cue:\t\t' + str(self.cueList[self.currentCue+1][0]))
        return 112

    def nextCue(self):
        if self.currentCue != self.cueListLength-1:
            self.currentCue += 1
            self.updateCueText()
            return True
        else: return False

    def previousCue(self):
        if self.currentCue != 0:
            self.currentCue -= 1
            self.updateCueText()
            return True
        else: return False


if __name__ == '__main__':
    root = Tk()
    root.geometry("400x200")
    root.resizable(width=False, height=False)
    app = Window(root)
    root.mainloop()
