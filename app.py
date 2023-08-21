import kivy
kivy.require('2.2.1') # replace with your current kivy version !

import os
import thsGen
import requests
from getSVG import getSVG
from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.image import Image
from kivy.uix.button import Button
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

def strucShow(self):
    App.get_running_app().root.get_screen("strucShowWindow").ids.strucShowBox.clear_widgets()

    for file in os.listdir():
        if file.startswith("struc_"):
            os.remove(file)

    print(self.text)
    fileName = getSVG(self.text)

    App.get_running_app().root.current = "strucShowWindow"
    App.get_running_app().root.get_screen("strucShowWindow").ids.strucLabel.text = f"polymer graph visual for: {self.text}"
    App.get_running_app().root.get_screen("strucShowWindow").ids.strucShowBox.add_widget(Image(source = f"struc_{fileName}.png", size_hint = (1, 1), allow_stretch = True))


class strucShowScreen(Screen):
    pass

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
        
        if len(self.children[0].children[1].children) > 4:
            for i in range(0, 4):
                self.ids.inputGrid.remove_widget(self.children[0].children[1].children[0])
        else:
            self.ids.promptLabel.text = "You must input 1 or more miRNA"

    def getMirBase(self):
        print("getting miRBase!")
        with open("mature.fa", 'wb') as f:
            f.write(requests.get("https://mirbase.org/download/mature.fa").content)
        self.manager.current = "mirBaseWindow"

    def clear(self):
        for i, child in enumerate(self.children[0].children[1].children):
            if (i % 2 == 0):
                child.text = ""
    
    def submit(self):
        print("submitted")
        
        print(Window.size[0])

        global manager
        manager = self.manager

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
                        thsGen.start()
                        thsGen.thsPlotSave()
                        thsGen.triggerPlotSave()
                        thsGen.complexPlotSave()
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
            

        # Toehold switch screen
        manager.get_screen("second").ids.thsBox.add_widget(Image(source = "ths.png"))

        thsGridText = BoxLayout(orientation = "vertical")
        manager.get_screen("second").ids.thsBox.add_widget(thsGridText)

        thsGridText.add_widget(Label(text = "Toehold switch sequence and Boltzmann sampled structures:"))
        thsGridText.add_widget(Label(text = thsGen.bestThs.sequence, font_size = 12, text_size = ((Window.size[0] - 200)/2, 50), halign = "center", valign = "top"))
        for x in thsGen.bestThs.thsSamples:
            thsGridText.add_widget(Button(text = str(x), font_size = 11, size_hint = (1, 1), on_press = strucShow))
        thsGridText.add_widget(Label(text = f"Toehold binding percentage of: {str(round(thsGen.bestThs.exposure, 2))}% Approximate minimum free energy of: {str(round(thsGen.bestThs.strucEnergy.energy, 2))} (kcal/mol)"))
        
        # Trigger screen
        if thsGen.numMiRNA > 1:
            manager.get_screen("third").ids.triggerBox.add_widget(Image(source = "trigger.png"))

            triggerGridText = BoxLayout(orientation = "vertical")
            manager.get_screen("third").ids.triggerBox.add_widget(triggerGridText)

            triggerGridText.add_widget(Label(text = "Trigger complex strand order and Boltzmann sampled structures:"))

            global bestThsOrderedStrands
            bestThsOrderedStrands = thsGen.listAddition(thsGen.bestThs.strandsUsed[0:int((len(thsGen.strands)-1)/2)], thsGen.strands[int((len(thsGen.strands)-1)/2):int(len(thsGen.strands))])
            triggerGridText.add_widget(Label(text = ", ".join(bestThsOrderedStrands), font_size = 12, text_size = ((Window.size[0] - 200)/2, 50), halign = "center", valign = "top"))
            for x in thsGen.bestThs.triggerSamples[:10]:
                triggerGridText.add_widget(Button(text = str(x), font_size = 11, size_hint = (1, 1), on_press = strucShow))
            triggerGridText.add_widget(Label(text = f"Approximate minimum free energy of: {str(round(thsGen.triggerMFEStruc.energy, 2))} (kcal/mol)"))
            for i, j in enumerate(bestThsOrderedStrands):
                if (i % 2 == 1):
                    triggerGridText.add_widget(Label(text = "".join((j, " is the and gate between ", thsGen.getKey(bestThsOrderedStrands[i-1]), " (", bestThsOrderedStrands[i-1], ") ", " and ", thsGen.getKey(bestThsOrderedStrands[i+1]), " (", bestThsOrderedStrands[i+1], ") ")), text_size = ((Window.size[0] - 200)/2, 50), font_size = 12, halign = "center", valign = "center"))
        else:
            print("no trigger for 1 miRNA")
            manager.get_screen("third").ids.triggerBox.add_widget(Label(text = "No trigger complex is needed for 1 miRNA strand", font_size = 20))


        # Complex screen
        manager.get_screen("fourth").ids.complexBox.add_widget(Image(source = "complex.png"))
        complexGridText = BoxLayout(orientation = "vertical")
        manager.get_screen("fourth").ids.complexBox.add_widget(complexGridText)
        complexGridText.add_widget(Label(text = "Activated toehold switch suboptimal energy structures:"))
        for x in thsGen.finalComplex.subopt[:10]:
            complexGridText.add_widget(Button(text = str(x.structure), font_size = 10, size_hint = (1, 1), on_press = strucShow))
        global bindingSites
        bindingSites = ""
        for x in thsGen.finalComplex.subopt:
            bindingSite = str(x.structure)[:int(str(x.structure).find("+"))]
            bindingSites += bindingSite
        global thsActivationString
        thsActivationString = "".join(("Toehold switch activation percentage: ", str(round(100-bindingSites.count(".")*100/len(bindingSites), 2)), "%", " Approximate free energy of: ", str(round(thsGen.finalComplex.mfe[0].energy, 2)), " (kcal/mol)"))
        complexGridText.add_widget(Label(text = thsActivationString))
        complexGridText.add_widget(Label(text = "*Toehold switch activation percentage is a measure of how many bases of the switch bind at equilibrium when the switch is open, it isn't a representation of what percentage of switches will be open", text_size = ((Window.size[0] - 200)/2, 50), font_size = 12)) 

    ev.bind(on_thsGen = received)

class thsWindow(Screen):
    pass

class triggerWindow(Screen):
    pass

class complexWindow(Screen):
    def resultsOutput(self):
        print("outputting txt file")
        outputFile = open("results.txt","w")
        outputFile.write("~~~~~~~~~~~~~~~~~Toehold Switch~~~~~~~~~~~~~~~~~ \n")
        outputFile.write(f"{thsGen.bestThs.sequence} \n")
        outputFile.write(f"MFE structure: {thsGen.bestThs.strucEnergy.structure} ({str(round(thsGen.bestThs.strucEnergy.energy, 2))} kcal/mol) with a toehold exposure of {str(round(thsGen.bestThs.exposure, 2))}% \n")
        outputFile.write(f"Boltzmann sampled structures: \n")
        for x in thsGen.bestThs.thsSamples:
            outputFile.write(f"{x} \n")
        outputFile.write("\n ~~~~~~~~~~~~~~~~~Trigger Complex~~~~~~~~~~~~~~~~~ \n")
        if thsGen.numMiRNA == 1:
            outputFile.write("There is no trigger complex for a toehold switch using 1 miRNA")
            for i, j in enumerate(bestThsOrderedStrands):
                if (i % 2 == 1):
                    outputFile.write(f'{"".join((j, " is the and gate between ", thsGen.getKey(bestThsOrderedStrands[i-1]), " (", bestThsOrderedStrands[i-1], ") ", " and ", thsGen.getKey(bestThsOrderedStrands[i+1]), " (", bestThsOrderedStrands[i+1], ") "))}')
        
        else:
            outputFile.write(f"Best strand order including and gate sequences: {', '.join(bestThsOrderedStrands)} \n")
            outputFile.write(f"Structure considering Boltzmann sampled structures: {thsGen.bestThs.triggerStruc} \n")
            outputFile.write(f"Structure considering MFE structure: {thsGen.triggerMFEStruc.structure} ({round(thsGen.triggerMFEStruc.energy, 2)} kcal/mol) \n")
            for i, j in enumerate(bestThsOrderedStrands):
                if (i % 2 == 1):
                    outputFile.write(f'{"".join((j, " is the and gate between ", thsGen.getKey(bestThsOrderedStrands[i-1]), " (", bestThsOrderedStrands[i-1], ") ", " and ", thsGen.getKey(bestThsOrderedStrands[i+1]), " (", bestThsOrderedStrands[i+1], ") "))} \n')
            outputFile.write("With these other possible structures*: \n")
        for x in thsGen.bestThs.triggerSamples:
            outputFile.write(f"{str(x)} \n")
        outputFile.write(f"*Several trigger Boltzmann samples are given in this file because the trigger structure tends to be much more volitile and subject to forming off-target complexes so 50 of the samples are shown here to assess how likely it is for the correct complex to form \n")
        outputFile.write(f"*MFE structure may not be fully representative of the trigger complex's structure, which is why Boltzmann samples are taken. If the MFE structure closely resembles the boltzmann sampled ones, it is extremely likely to form, if not it will still be possible although at lower concentrations \n")
        outputFile.write(f"*Boltzmann samples will be more reliable and representative of in vitro reactions as hundreds of samples are taken, MFE structure is only taken into account to give an approximate free energy value and equilibrium pair probability matrix \n")
        outputFile.write("\n ~~~~~~~~~~~~~~~~~Activated Toehold Switch~~~~~~~~~~~~~~~~~ \n")
        outputFile.write(f"MFE structure: {thsGen.finalComplex.mfe[0].structure} ({round(thsGen.finalComplex.mfe[0].energy, 2)} kcal/mol) \n")
        outputFile.write(f"Toehold switch activation percentage: {round(100-bindingSites.count('.')*100/len(bindingSites), 2)}% \n")
        outputFile.write("Suboptimal energy structures: \n")
        for x in thsGen.finalComplex.subopt:
            outputFile.write(f"{x.structure} ({round(x.energy, 2)} kcal/mol) \n")
        outputFile.write("Toehold switch activation percentage is a measure of how many bases of the switch bind at equilibrium when the switch is open, it isn't a representation of what percentage of switches will be open")
        outputFile.write("Here suboptimal energy structures are used as they are incredibly similar to taking Boltzmann samples while also giving a free energy value")


    addButton = Button(text = "Add miRNA")
class mirBaseWindow(Screen):

    global names, sequences
    names = []
    sequences = []
    
    def search(self):

        self.ids.searchGrid.clear_widgets()

        global manager
        manager = self.manager

        global names, sequences                
        
        def removemiRNA(self):
            print("removing miRNA")
            selfIdx = manager.get_screen("mirBaseWindow").ids.mirnaSelectBox.children.index(self)

            manager.get_screen("mirBaseWindow").ids.mirnaSelectBox.remove_widget(manager.get_screen("mirBaseWindow").ids.mirnaSelectBox.children[selfIdx])
            manager.get_screen("mirBaseWindow").ids.mirnaSelectBox.remove_widget(manager.get_screen("mirBaseWindow").ids.mirnaSelectBox.children[selfIdx])

            print(names)
            for (label, button) in zip([x.text for x in manager.get_screen("mirBaseWindow").ids.searchGrid.children[3::4]], manager.get_screen("mirBaseWindow").ids.searchGrid.children[0::4]):
                if label == names[(len(names) - 1) - int(selfIdx/2)]:
                    button.disabled = False
            
            del names[(len(names) - 1) - int(selfIdx/2)]
            del sequences[(len(names) - 1) - int(selfIdx/2)]

            

        def addmiRNA(self):
            print("adding miRNA")
            if len(names) < 3:
                self.disabled = True
                selfIdx = manager.get_screen("mirBaseWindow").ids.searchGrid.children.index(self)
                name = manager.get_screen("mirBaseWindow").ids.searchGrid.children[selfIdx + 3].text
                sequence = manager.get_screen("mirBaseWindow").ids.searchGrid.children[selfIdx + 1].text
                names.append(name)
                sequences.append(sequence.strip())
                manager.get_screen("mirBaseWindow").ids.mirnaSelectBox.add_widget(Label(text = name, size_hint = (0.75, 1/3)))
                manager.get_screen("mirBaseWindow").ids.mirnaSelectBox.add_widget(Button(text = "remove", font_size = 12, size_hint = (0.25, 1/3), on_press = removemiRNA))
                
            else:
                manager.get_screen("mirBaseWindow").ids.mirnaSelectText.text = "> 3 miRNA is not supported" 
        
        if self.children[0].children[-1].children[1].text:
            print("searching for miRNA")
            # print(self.children[0].children[-1].children[1].text)
            miRBase = open("mature.fa")
            query = self.children[0].children[-1].children[1].text
            for i, x in enumerate(miRBase.readlines()):
                if x.startswith(">hsa"):
                    if query in x.split(" ")[0]:
                        name = x.split(" ")[0][1:]
                        accession = x.split(" ")[1]
                        sequence = open("mature.fa").readlines()[i+1]
                        self.ids.searchGrid.add_widget(Label(text = name, size_hint_y = None, size = (10, 20), size_hint = (0.2, None)))
                        self.ids.searchGrid.add_widget(Label(text = accession, size_hint_y = None, size = (10, 20), size_hint = (0.2, None)))
                        self.ids.searchGrid.add_widget(Label(text = sequence, size_hint_y = None, size = (10, 20), size_hint = (0.4, None)))
                        self.ids.searchGrid.add_widget(Button(text = "Add miRNA", size = (10, 50),  size_hint = (0.2, None), on_press = addmiRNA))
        
        for name in names:
            for (label, button) in zip([x.text for x in manager.get_screen("mirBaseWindow").ids.searchGrid.children[3::4]], manager.get_screen("mirBaseWindow").ids.searchGrid.children[0::4]):
                if name == label:
                    button.disabled = True

        
    
    def submitFromBase(self):
        if self.manager.get_screen("mirBaseWindow").ids.mirnaSelectBox.children:
            print("submitting miRNA")
            print(names)
            print(sequences)
            self.ids.mirBaseSubmitButton.disabled = True
            submitDict = dict(zip(names, sequences))  
            thsGen.miRNADict = submitDict
            print(thsGen.miRNADict)
            thsGen.start()
            thsGen.thsPlotSave()
            thsGen.triggerPlotSave()
            thsGen.complexPlotSave()
            self.ids.nextButton2.disabled = False
            ev.dispatch("on_thsGen")
        else:
            print("input something, dingus")

    
class ToeholdSwitchGenerator(App):

    def build(self):
        
        Window.clearcolor = (10/225.0, 22/255.0, 40/255.0, 1)
        sm = ScreenManager()
        sm.add_widget(mainWindow())
        sm.add_widget(thsWindow())
        sm.add_widget(triggerWindow())
        sm.add_widget(complexWindow())
        sm.add_widget(mirBaseWindow())
        sm.add_widget(strucShowScreen())

        return sm


if __name__ == '__main__':
    ToeholdSwitchGenerator().run()