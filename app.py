import kivy
kivy.require('2.2.1') # replace with your current kivy version !

from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.gridlayout import GridLayout
from kivy.uix.image import Image
from kivy.uix.button import Button
from kivy.uix.textinput import TextInput
from kivy.uix.screenmanager import Screen, ScreenManager

class mainScreen(Screen):
    pass

class resultScreen(Screen):
    pass

class screenManager(ScreenManager):
    pass

class mainPage(GridLayout):
    def __init__(self, **kwargs):
        super(mainPage, self).__init__(**kwargs)
        
        self.cols = 1

        self.inputLayout = GridLayout()
        self.inputLayout.cols = 4

        self.add_widget(Image(source='GENOSWITCH_logo_Coloured.png'))

        self.addMiRNAButton = Button(text = 'add miRNA')
        self.addMiRNAButton.bind(on_press = self.addMiRNA)
        self.add_widget(self.addMiRNAButton)

        self.inputLayout.add_widget(Label(text='miRNA name'))
        self.inputLayout.add_widget(TextInput(multiline=False))

        self.inputLayout.add_widget(Label(text='miRNA sequence'))
        self.inputLayout.add_widget(TextInput(multiline=False))

        self.add_widget(self.inputLayout)

        self.submitButton = Button(text='submit')
        self.submitButton.bind(on_press = self.submit)
        self.add_widget(self.submitButton)

    def addMiRNA(self, instance):
        self.inputLayout.add_widget(Label(text='miRNA name'))
        self.inputLayout.add_widget(TextInput(multiline=False))

        self.inputLayout.add_widget(Label(text='miRNA sequence'))
        self.inputLayout.add_widget(TextInput(multiline=False))
    
    def submit(self, instance):
        contents = []
        for i, child in enumerate(self.inputLayout.children):
            print(i, child.text)
            if (i % 2 == 0):
                print('required')
                contents.append(child.text)
        print(contents)
        print(contents[::2])
        print(contents[1:][::2])
        dict = {contents[1:][::2][i]: contents[::2][i] for i in range(len(contents[1:][::2]))}
        print(dict)




class MyApp(App):

    def build(self):
        return mainPage()


if __name__ == '__main__':
    MyApp().run()