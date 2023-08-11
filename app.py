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

class mainWindow(Screen):

    def addMiRNA(self):
        print("miRNA added")
        self.ids.inputGrid.add_widget(Label(text='miRNA name'))
        self.ids.inputGrid.add_widget(TextInput(multiline=False))

        self.ids.inputGrid.add_widget(Label(text='miRNA sequence'))
        self.ids.inputGrid.add_widget(TextInput(multiline=False))
    
    def submit(self):
        print("submitted")
        # print(self.children[0])
        # print(self.children[0].children)
        # print(self.children[0].children[2].children)

        contents = []
        for i, child in enumerate(self.children[0].children[2].children):
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
                        dict = {miRNANames[i]: miRNASequences[i] for i in range(len(contents[1:][::2]))}
                        thsGen.miRNADict = dict
                        self.ids.promptLabel.text = "computing..."
                        thsGen.start()
                    else:
                        self.ids.promptLabel.text = "sorry, our software doesn't support miRNA strands shorter than 15nt"
                else:
                    self.ids.promptLabel.text = "wrong input"
            else:
                self.ids.promptLabel.text = "multipule miRNA strands cannot share the same name or sequence"
        else:
            self.ids.promptLabel.text = "please fill in all of the boxes"





class thsWindow(Screen):
    pass

class triggerWindow(Screen):
    pass

class complexWindow(Screen):
    pass

class ToeholdSwitchGenerator(App):

    def build(self):
        
        Window.clearcolor = (10/225, 22/255, 40/255, 1)
        sm = ScreenManager()
        sm.add_widget(mainWindow())
        sm.add_widget(thsWindow())
        sm.add_widget(triggerWindow())
        sm.add_widget(complexWindow())



        return sm


if __name__ == '__main__':
    ToeholdSwitchGenerator().run()