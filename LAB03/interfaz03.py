from tkinter import *

window = Tk()

window.title("Interfaz Grafica")

window.geometry('350x200')



btn = Button(window, text="Conectar", bg="green", fg="white")

btn.grid(column=0, row=1)

lbl = Label(window, text="Contador")

lbl.grid(column=0, row=3)

txt = Entry(window,width=10)
txt.grid(column=0, row=5)
btnEnv = Button(window, text="Enviar", bg="blue", fg="white")

btnEnv.grid(column=2, row=5)

btn2 = Button(window, text="Pot1: 0.00V", bg="orange", fg="white")

btn2.grid(column=1, row=7)

btn3 = Button(window, text="Pot2: 0.00V", bg="orange", fg="white")

btn3.grid(column=2, row=7)

window.mainloop()