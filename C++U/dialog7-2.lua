

RObjectText =
{  
  helmet = "Acquired:Rock helmet" 
} 

Dialogue =
{
  --0 到 8
  { tag=2, text ="Dick sent me here waiting for you, Mr.Big head", font = Font.default, } ,
  { tag=1, text ="It seems you ate tons of garlics, Mr.Poo licker，", font = Font.default },
  { tag=1, text ="What is your trick at this time?", font = Font.default }, 
  { tag=2, text ="Since you got tough body,\n I don' know whether your head as tough as your body is", font = Font.default },
  { tag=2, text ="I got a Q&A game here,use your head,\nif your answer is right, you got a gift! ", font = Font.default },
  { tag=1, text ="What if i am wrong?",font = Font.default },
  { tag=2, text ="You will also get \"a big reward\"",font = Font.default },
  { tag=1, text ="All right!",font = Font.default },
  { tag=2, text ="Enough chitchat, Here are the questions",font = Font.default },
  { tag=1, text ="Bring it on!!",font = Font.default },
}


Question =
{
  {text=[[Question1：1 + 1 = ? 
A.2 B.1 C.22 D.11 ]], font = Font.default,key='A',   correct =  {text="Pity，you are right -_-", font=Font.default,}, 
    incorrect= {text="Congratulations，you are wrong！", font=Font.default,},},
  
  {text=[[Question2：
1+1+1+1+1
1+1+1+1+1
1+1*0+1=?
A.11 B.12 C.13 D.30]], font = Font.default,key='D', correct =  {text="Sad，correct -_-", font=Font.default,}, incorrect= {text="feel really good！", font=Font.default,},},

  {text=[[Question3：Who is the most handsome man in the game
A.Poo licker B.Devil Dick C.The slime D.Jacky.Ball ]], font = Font.default,key='A', correct =  {text="Correct,here coming the gift", font=Font.default,}, incorrect= {text="Wrong,maybe you should learn kiss how to someone's ass", font=Font.default,},},

  {text=[[Question4: What do you think about those ball-like people in the game
A.They are bastards。
B.They are retard,stupied,low-end degenerates!
D.They are being brain-washed by Devil dick ]], font = Font.default,key='D', correct =  {text="Bingo，again！", font=Font.default,}, incorrect= {text="Congratulations，you are wrong！", font=Font.default,},},

  {text=[[Question5：What is a type of specice like devil Dick：
B.A turtle like vampire.
D.A cheating clown came from nowhere.
C.Include all above,It is also a representative of evil force, hypocrisy,autocracy.]], font = Font.default,key='C', correct =  {text="Excellent，another one", font=Font.default,}, incorrect= {text="With respect, you're wrong", font=Font.default,},},

  {text=[[The last one：
What is meaning of number 250 ?
A. Goodwill B. locations C.Tags D.Time ]], font = Font.default,key='NoKey', correct =  {text="250", font=Font.default,}, incorrect= {text="Congratulations, you got a 250", font=Font.default,},},
}
 
 