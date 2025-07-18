require("global")
Hints =
{  
  grumble =
  {
    { text ="钢蛋救命！！", font = Font.default, } ,
    { text ="饶命啊,我把钱全给你", font = Font.default, } ,
    { text ="我还年轻不想死！！ 救我", font = Font.default, } ,
    { text ="快把我放出去！！", font = Font.default, } ,
    { text ="救命！！", font = Font.default, } ,
  },
  dialogBoard =  { text ="坦坦荡荡做人\n光明磊落做事", font = Font.default, } ,
  correctForShooting =  { text ="恭喜您:\n全部答对，请尽情享用", font = Font.default, } ,
  finishForShooting =  { text ="恭喜您：\n回答完毕， 请尽情享用！", font = Font.default, } ,
} 

Dialogue =
{
  --0 到 5
  { tag=2, text ="这次你小子必须把水晶交出来！", font = Font.default, } ,
  { tag=1, text ="没门！", font = Font.default },
  { tag=2, text ="否则的话，这些混蛋的小命就没了", font = Font.default }, 
  { tag=1, text ="好卑鄙啊，迪克！", font = Font.default },
  { tag=2, text ="我可是身经百战了啊\n 你毕竟还是too young, \n除非你能答对我所有的问题,\n 我就放了他们", font = Font.default },
  { tag=1, text ="看来只好尽力而为了！",font = Font.default },

}

Question =
{
  {text=[[第1题： 既然你已经拿到水晶，
请问水晶一共几种颜色？
A.6 B.5 C.4]], font = Font.default,key='A',  correct =  {text="真失败，竟然答对了。", font=Font.default,},  incorrect= {text="恭喜您，答错了！", font=Font.default,},},

  {text=[[第2题：彩虹有7种颜色，
水晶比彩虹少一种。是哪种颜色
A.褐色 B.黑色 C.紫色]], font = Font.default,key='C', correct =  {text="不可思议，竟然答对了。", font=Font.default,}, incorrect= {text="回答错误！降落", font=Font.default,},},
  {text=[[第3题： 谁是这个星球上最帅的人？
A.史莱姆 B.汪思甜 C.迪克大魔王]], font = Font.default,key='C', correct =  {text="不错~ 看来你跟我家狗狗没白学。", font=Font.default,}, incorrect= {text="答错了！开闸！", font=Font.default,},},
  
  {text=[[第4题：本王预言星球将会发生一场
翻天覆地的变革，你认为将会在那一年？
A.2018 B.2022 C.3250 D.5000 ]], font = Font.default,key='B', correct =  {text="真失败，答对了。", font=Font.default,}, incorrect= {text="恭喜您，答错了！", font=Font.default,},},
  
  {text=[[第5题：请最后一个混蛋选择一个幸运数字
A.666 B.888 C.444 D.250]], font = Font.default,key='D', correct =  {text="恭喜贺喜，答对了", font=Font.default,}, incorrect= {text="这个数字不合适！", font=Font.default,},},

}


