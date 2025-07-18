 
Hints =
{  
  grumble =
  {
    { text ="Save us, Jacky!", font = Font.default, } ,
    { text ="Please, I will give you all of my money", font = Font.default, } ,
    { text ="I'am young,I don't wanna die", font = Font.default, } ,
    { text ="Get me out of here", font = Font.default, } ,
    { text ="Help!!!", font = Font.default, } ,
  },
  dialogBoard =  { text ="Be honest \n,open&aboveboard", font = Font.default, } ,

    correctForShooting =  { text ="congratulations\nyou got perfect score,\nenjoy the dish", font = Font.default, } ,
  finishForShooting =  { text ="Good job.\nhere is the reward.", font = Font.default, } ,
} 

Dialogue =
{
  --0 到 5
  { tag=2, text ="You must give my crystal back", font = Font.default, } ,
  { tag=1, text ="No way!", font = Font.default },
  { tag=2, text ="Or, these bastard will die", font = Font.default }, 
  { tag=1, text ="You are really nasty, vicious. Dick!!!", font = Font.default },
  { tag=2, text ="You are after all too young, \n unless you answer my questions correctly, \nI will release them.", font = Font.default },
  { tag=1, text ="It looks like I got no choice.",font = Font.default },

}

Question =
{
  {text=[[Question 1： Since you've got the crystal, 
how many colors of it?
A.6 B.5 C.4]], font = Font.default,key='A',  correct =  {text="Right, next one。", font=Font.default,},  incorrect= {text="Wrong!", font=Font.default,},},

  {text=[[Question 2：Which color should be
added into the crystal?
A. Black B.White  C.Purple]], font = Font.default,key='C', correct =  {text="Unbelievable，correct。", font=Font.default,}, incorrect= {text="Incorrect!", font=Font.default,},},
  {text=[[Question 3： Who is the most handsome people in the game
A.Slimes B.The dog(poo licker) C.Dick]], font = Font.default,key='C', correct =  {text="Good,It seems you've \nlearnt a lot from my dog", font=Font.default,}, incorrect= {text="Wrong,fall!", font=Font.default,},},
  
  {text=[[Question4：I predict there will be a huge revolution
on the planet, when it is gonna happen?
A.2018 B.2022 C.3250 D.5000 ]], font = Font.default,key='B', correct =  {text="Smart, you are right", font=Font.default,}, incorrect= {text="Incorrect ,Boy! ", font=Font.default,},},
  
  {text=[[The last one:Please chose
a lucky number for you
A.5 B.4 C.23 D.250]], font = Font.default,key='D', correct =  {text="Congratulations,wise choice!", font=Font.default,}, incorrect= {text="That was inappropriate！", font=Font.default,},},

}




