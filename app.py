import kivy
kivy.require('2.2.1') # replace with your current kivy version !

import thsGen
from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.uix.screenmanager import Screen, ScreenManager
from kivy.lang import Builder
from kivy.core.window import Window

kv = Builder.load_file("thsGenerator.kv")

Window.size = (800,550)

class mainWindow(Screen):

    def addMiRNA(self):
        print("miRNA added")

        if len(self.children[0].children[1].children) != 12:
            self.ids.inputGrid.add_widget(Label(text='miRNA name'))
            self.ids.inputGrid.add_widget(TextInput(multiline=False))

            self.ids.inputGrid.add_widget(Label(text='miRNA sequence'))
            self.ids.inputGrid.add_widget(TextInput(multiline=False))
        else:
            self.ids.promptLabel.text = "The software does not support more than 3 miRNA strands"
    
    def removeMiRNA(self):
        print("miRNA removed")
        # print(self.children[0])
        # print(self.children[0].children)
        # print(self.children[0].children[1].children)
        if len(self.children[0].children[1].children) > 4:
            for i in range(0, 4):
                self.ids.inputGrid.remove_widget(self.children[0].children[1].children[-1])
        else:
            self.ids.promptLabel.text = "You must input 1 or more miRNA"

    
    def clear(self):
        for i, child in enumerate(self.children[0].children[1].children):
            if (i % 2 == 0):
                child.text = ""
    
    def submit(self):
        print("submitted")
        # print(self.children[0])
        # print(self.children[0].children)
        # print(self.children[0].children[1].children)

        contents = []
        for i, child in enumerate(self.children[0].children[1].children):
            if (i % 2 == 0):
                contents.append(child.text)

        if all([i != "" for i in contents]):

            miRNANames = contents[1:][::2]
            namesCheckSet = set(miRNANames)

            miRNASequences = contents[::2]
            seqCheckSet = set(miRNASequences)

            if (len(seqCheckSet) == len(miRNASequences) and len(namesCheckSet) == len(miRNANames)):

                checkList = [thsGen.checkCorrectCharacters(i) for i in miRNASequences]

                if all(checkList):

                    if all(len(i) > 14 for i in miRNASequences):
                        dict = {miRNANames[i]: str(miRNASequences[i]).upper() for i in range(len(contents[1:][::2]))}
                        thsGen.miRNADict = dict
                        print(thsGen.miRNADict)
                        self.ids.promptLabel.text = "Computing..."
                        # thsGen.start()
                    else:
                        self.ids.promptLabel.text = "Sorry, our software doesn't support miRNA strands shorter than 15nt"
                else:
                    self.ids.promptLabel.text = "One or more of your sequences contains illegal characters"
            else:
                self.ids.promptLabel.text = "Multipule miRNA strands cannot share the same name or sequence"
        else:
            self.ids.promptLabel.text = "Please fill in all of the boxes"





class thsWindow(Screen):
    pass

class triggerWindow(Screen):
    pass

class complexWindow(Screen):
    pass

class ToeholdSwitchGenerator(App):

    def build(self):
        
        Window.clearcolor = (10/225.0, 22/255.0, 40/255.0, 1)
        sm = ScreenManager()
        sm.add_widget(mainWindow())
        sm.add_widget(thsWindow())
        sm.add_widget(triggerWindow())
        sm.add_widget(complexWindow())

        return sm


if __name__ == '__main__':
    ToeholdSwitchGenerator().run()