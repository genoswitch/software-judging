import kivy
kivy.require('2.2.1') # replace with your current kivy version !

import thsGen
from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.image import Image
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivy.uix.screenmanager import Screen, ScreenManager
from kivy.lang import Builder
from kivy.core.window import Window
from kivy.event import EventDispatcher

kv = Builder.load_file("thsGenerator.kv")

Window.size = (1500,800)

class MyEventDispatcher(EventDispatcher):
    def __init__(self, **kwargs):
        self.register_event_type('on_thsGen')
        super(MyEventDispatcher, self).__init__(**kwargs)

    def on_thsGen(self):
        pass
    
ev = MyEventDispatcher()

class mainWindow(Screen):

    def addMiRNA(self):
        print("miRNA added")

        if len(self.children[0].children[1].children) != 12:
            self.ids.inputGrid.add_widget(Label(text='miRNA name'))
            self.ids.inputGrid.add_widget(TextInput(multiline=False, font_size = 18))

            self.ids.inputGrid.add_widget(Label(text='miRNA sequence'))
            self.ids.inputGrid.add_widget(TextInput(multiline=False, font_size = 18))
        else:
            self.ids.promptLabel.text = "The software does not support more than 3 miRNA strands"
    
    def removeMiRNA(self):
        print("miRNA removed")
        # print(self.children[0])
        # print(self.children[0].children)
        # print(self.children[0].children[1].children)
        if len(self.children[0].children[1].children) > 4:
            for i in range(0, 4):
                self.ids.inputGrid.remove_widget(self.children[0].children[1].children[0])
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

        print(self)
        global manager
        manager = self.manager
        print(manager)

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
                        self.ids.submitButton.disabled = True
                        dict = {miRNANames[i]: str(miRNASequences[i]).upper() for i in range(len(contents[1:][::2]))}
                        thsGen.miRNADict = dict
                        print(thsGen.miRNADict)
                        print(thsGen.isDone)
                        thsGen.start()
                        thsGen.thsPlotSave()
                        thsGen.triggerPlotSave()
                        thsGen.complexPlotSave()
                        print(thsGen.isDone)
                        self.ids.promptLabel.text = "Done!"
                        self.ids.nextButton1.disabled = False
                        ev.dispatch("on_thsGen")
                    else:
                        self.ids.promptLabel.text = "Sorry, our software doesn't support miRNA strands shorter than 15nt"
                else:
                    self.ids.promptLabel.text = "One or more of your sequences contains illegal characters"
            else:
                self.ids.promptLabel.text = "Multipule miRNA strands cannot share the same name or sequence"
        else:
            self.ids.promptLabel.text = "Please fill in all of the boxes"
    
    def received(self):
        print("test event received")  

        manager.get_screen("second").ids.thsBox.add_widget(Image(source = "ths.png"))

        thsGridText = BoxLayout(orientation = "vertical")
        manager.get_screen("second").ids.thsBox.add_widget(thsGridText)

        thsGridText.add_widget(Label(text = "Toehold switch sequence and Boltzmann sampled structures:"))
        thsGridText.add_widget(Label(text = thsGen.bestThs.sequence, font_size = 10))
        for x in thsGen.bestThs.thsSamples:
            thsGridText.add_widget(Label(text = str(x), font_size = 12))
        thsGridText.add_widget(Label(text = f"Toehold binding percentage of: {str(round(thsGen.bestThs.exposure, 2))} Approximate minimum free energy of: {str(round(thsGen.bestThs.strucEnergy.energy, 2))}"))
        
        manager.get_screen("third").ids.triggerBox.add_widget(Image(source = "trigger.png"))

        triggerGridText = BoxLayout(orientation = "vertical")
        manager.get_screen("third").ids.triggerBox.add_widget(triggerGridText)

        triggerGridText.add_widget(Label(text = "Trigger complex strand order and Boltzmann sampled structures:"))
        bestThsOrderedStrands = thsGen.listAddition(thsGen.bestThs.strandsUsed[0:int((len(thsGen.strands)-1)/2)], thsGen.strands[int((len(thsGen.strands)-1)/2):int(len(thsGen.strands))])
        triggerGridText.add_widget(Label(text = str(bestThsOrderedStrands), font_size = 10))
        for x in thsGen.bestThs.triggerSamples:
            triggerGridText.add_widget(Label(text = str(x), font_size = 12))
        triggerGridText.add_widget(Label(text = f"Approximate minimum free energy of: {str(round(thsGen.triggerMFEStruc.energy, 2))}"))
        for i, j in enumerate(bestThsOrderedStrands):
            if (i % 2 == 1):
                print(j, " is the and gate between ", thsGen.getKey(bestThsOrderedStrands[i-1]), " (", bestThsOrderedStrands[i-1], ") ", " and ", thsGen.getKey(bestThsOrderedStrands[i+1]), " (", bestThsOrderedStrands[i+1], ") ")
        
        

    ev.bind(on_thsGen = received)






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