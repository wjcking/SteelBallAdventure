local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local State = require("state")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State1 = { Player={}, Npc={},RObject={}, ProjectTile={},Wangba={},Dialog={}, Trigger={},Stage={}}

State1.Trigger.Block = State:new()
State1.Trigger.Block.execute = function(trigger)

  if (trigger.isActive ~= nil and not trigger.isActive) then
    return
  end

  if (not trigger.ref:scan(Role[1].ref)) then
    Role[1].ref:closeDialog()
    return
  end    
  Role[1].ref:showHints(Hints.block)

  if (trigger.isBlocked == nil) then
    Role[RoleIndex.board].ref:registerText(RObjectText.block)

    trigger.isBlocked = false
  end

end
--***********按钮事件************
State1.RObject.Button = State:new()
State1.RObject.Button.execute = function(object)

  object.getPlayer(object,function(range,player)

      if (range < RangeType.rangeThreatened) then 
        return
      end

      local switchResult = object.ref:switchFrame(Role[1].ref)
      --armeabi gcc 不能用父类FrameIndexes注册lua所以声明此if (switchResult.frameIndex > 1) then
      if (switchResult:getIndex() > 1 ) then

        if (object.tag == 2) then
          Role[5].ref:registerText(RObjectText.website)
        elseif(object.tag == 3) then
          Triggers[2].fsm:clearGlobal()
          getStage():removeTileRange(Vec2(2,15),Vec2(3,16))
          object.fsm:clearState() 
          Resh.playSound(Sound.enigma)
        elseif(object.tag == 4) then 
          Role[5].ref:registerText(RObjectText.contact)
        end
      end 
    end) 
end
--玩家钢蛋
State1.Player.Routine = State:new()
State1.Player.Routine.execute = function(player)
  getStage():setCameraSlide(player.ref:getPosition(),CameraView.vertical)
  --Role[1].ref:limiteScreenBorder(BorderLimited.limiteVertical)
  PlayerStates.Routine.execute(player)
end
State1.Player.Routine.onMessage = function(player, telegram)

  if (telegram.message == MessageType.trigger) then 
    if (telegram.sender.name == "block") then
      --  
      --player.ref:setAnimation(Animate.getToast("????????????"))

      Role[RoleIndex.board].ref:registerText(RObjectText.block)
      Role[5].ref:allowPush(CollisionDirection.atBottom)
      Role[5].fsm:changeState(RObjectStates.gotPushed)
    end

    if (telegram.sender.name == "start") then
      Joystick.ref:hideRocker()
      player.ref:freeze()
      player.ref:refreshOrigin()
      player.ref.moveStep = Vec2(0.5,0.5)
      player.fsm:changeState(State1.Player.Start)
    end
  end
  return false
end
State1.Wangba.Routine = State:new()
State1.Wangba.Routine.execute = function(npc)

end
--npc
State1.Npc.Routine = State:new()
State1.Npc.Routine.execute = function(npc)

end
State1.Npc.Routine.onMessage = function(npc, telegram)
  if (telegram.message == MessageType.hurting) then

    if (telegram.info  <= 1) then
      npc.ref:setAnimation(Animate.getToastDelay("￥-"..math.random(0,100),2)) 
    elseif (telegram.info == 2) then
      npc.ref:setAnimation(Animate.getToastDelay("$-"..math.random(0,100),2)) 
    elseif (telegram.info >= 3 and telegram.info <= 4) then
      npc.ref:setAnimation(Animate.getToastDelay("HP-250",2)) 
    end

    --如果是男性
    if (npc.framePrefix == "man") then
      npc.ref:gotHurt(0,Sound.manHurt1)
    else
      npc.ref:gotHurt(0,Sound.womanHurt1)
    end
  end

  if (telegram.message == MessageType.dead) then
    --如果是男性,死亡声音
    if (npc.framePrefix == "man") then
      npc.ref:gotoHell(Sound.manHurt1)
      --死亡后尸体随机翻转
      npc.ref.isFlippedX = false

    else
      npc.ref:gotoHell(Sound.womanHurt1) 
      npc.ref.isFlippedX = true
      --死亡后尸体随机翻转
    end
    npc.ref:stopAnimation()
    npc.ref:setAnimation({ type = Anistyle.fade, delay=3, delayTime=3})
    --钢蛋挑衅
    CommonStates.Delay.fsm=Segment[3].fsm
    CommonStates.Delay.state=State1.Player.Talk
    Segment[3].fsm:changeState(CommonStates.Delay)
    --王八放下枪
    Role[RoleIndex.wangba].fsm:clearState()
  end
  return false
end

--1） 玩家站在位置上
State1.Player.Start = State:new()
State1.Player.Start.execute = function(player)

  local isDestiny = player.ref:locateTile(Vec2(6,11),true)

  if (isDestiny) then
    player.ref.face = MovingDirection.toTop
    --clear bastardandcrystal 
    for i=6,RoleIndex.bastardEnd,1 do  
      Role[i].fsm:clearState()
    end 

    Segment[1].fsm:changeState(State1.Npc.Pray)
    player.fsm:clearState()
  end
end

--2）混蛋们开始许愿
State1.Npc.Pray = State:new()
State1.Npc.Pray.enter = function(segment)
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end
State1.Npc.Pray.execute = function(segment)
  DialogueSystem.update()

  if (DialogueSystem.isDone()) then
    segment.fsm:clearState()
    --crystal
    Role[21].fsm:changeState(State1.RObject.CrystalCrush)

  end
end
State1.Npc.Pray.exit = function(segment)
  DialogueSystem.close()
end
--3）水晶事件
State1.RObject.CrystalCrush = State:new()
State1.RObject.CrystalCrush.execute = function(object)
  local delayBroken = 2

  if (not Delay.isTimeUp(delayBroken, 1.3)) then
    return
  end
  object.ref:stopAnimation()
  --注意png
  object.ref:setFrame(object.framePrefix.."2")
  object.ref:gotoHell(Sound.crystalCrushed)
  object.ref:setAnimation(Animate.fadeCrystal)
  --王八行动
  Role[RoleIndex.wangba].fsm:changeState(State1.Npc.Wangba)
  --诡异的音乐
  Resh.playMusic(Music.dungeonBoss) --奸笑
  Resh.playSound(Sound.wangbaLaugh)
  object.fsm:clearState()
end

--4）王八袭来
State1.Npc.Wangba = State:new() 
State1.Npc.Wangba.enter = function(npc)
  --广告位
  Role[24].ref:registerText(RObjectText.adDick)
end
State1.Npc.Wangba.execute = function(npc)

  local isDestiny = npc.ref:locateTile(Vec2(9,8),true)
  --到后
  if (isDestiny) then
    npc.fsm:clearState()
    Segment[2].fsm:changeState(State1.Npc.WangbaTalk)
  end
end
--王八宣传了
State1.Npc.WangbaTalk = State:new()

State1.Npc.WangbaTalk.enter = function(segment)
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end 
State1.Npc.WangbaTalk.execute = function(segment)
  DialogueSystem.update()

  if (DialogueSystem.isDone()) then
    segment.fsm:clearState() 
    Role[22].fsm:changeState(State1.Npc.WangbaAttack)
  end
end
State1.Npc.WangbaTalk.exit = function(segment)
  DialogueSystem.close()
end
--王八攻击混蛋
State1.Npc.WangbaAttack = State:new()
State1.Npc.WangbaAttack.enter = function(npc)
  local attackTimes = 1
  local messageType = MessageType.hurting
  while (attackTimes <= 5) do
    npc.getOpponents(npc,function(range,opponent) 
        if (attackTimes == 5)  then
          messageType = MessageType.dead
        end
        if (opponent.type == RoleType.npc) then
          MsgDispatcher.dispatch(2.6 * (attackTimes -1),npc,opponent,messageType,attackTimes)
        end 
      end) 
    attackTimes = attackTimes + 1
  end
end
State1.Npc.WangbaAttack.execute = function(npc)
  --设置为开火状态但不发射子弹
  npc.ref:getWeapon():setFireOn()
end

--钢蛋挑衅
State1.Player.Talk = State:new()
State1.Player.Talk.enter = function(segment)
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end 
State1.Player.Talk.execute = function(segment)
  DialogueSystem.update()
  if (DialogueSystem.isDone()) then
    segment.fsm:clearState()
    Role[1].fsm:changeState(State1.Player.ReadyForBattle)
  end
end
State1.Player.Talk.exit = function(segment)
  DialogueSystem.close()
end

State1.Player.Fight = State:new()
State1.Player.Fight.enter  = function(player)

end 
--准备战斗
State1.Player.ReadyForBattle = State:new()
State1.Player.ReadyForBattle.enter = function(player)
  --拉下上下幕帘
  local curtainInfo = { image={"background/curtaindrop_dialog.png","background/curtaindrop.png"},text="", delay = 2, zorder = ZOrder.robject }
  Ref.stage:dropCurtain(curtainInfo)
  Role[1].ref.hp=1000
  Role[22].ref.hp=2000 
  Joystick.ref:setString(CommandPad.barSteelball,RObjectText.barSteelball.text..tostring(Role[1].ref.hp))
  Joystick.ref:setString(CommandPad.barDick,RObjectText.barDick.text..tostring(Role[22].ref.hp))

  Joystick.ref:setString(CommandPad.attack1,RObjectText.attack1.text)
  Joystick.ref:setString(CommandPad.attack2,RObjectText.attack2.text)
  Joystick.ref:setString(CommandPad.hand,RObjectText.hand.text)
  Joystick.ref:setString(CommandPad.dodge,RObjectText.dodge.text)
  Joystick.ref:setString(CommandPad.escape,RObjectText.escape.text) 
end
State1.Player.ReadyForBattle.execute = function(player)
  if (Delay.isTimeUp(4,3)) then
    Joystick.ref:setAllVisible(true)
    player.fsm:changeState(State1.Player.Battle)
    Role[22].fsm:changeState(State1.Npc.WangbaBattle)
  end
end
State1.setPadVisible = function(flag)
  Joystick.ref:setVisible(CommandPad.attack1,flag)
  Joystick.ref:setVisible(CommandPad.attack2,flag)
  Joystick.ref:setVisible(CommandPad.hand,flag)
  Joystick.ref:setVisible(CommandPad.dodge,flag)
  Joystick.ref:setVisible(CommandPad.escape,flag)
end

State1.Player.Battle = State:new()
State1.Player.Battle.enter = function(player)
  --windows调试可以走动
  player.battleCommand = nil
  player.ref:unfreeze()
end
State1.Player.Battle.execute = function(player)

  local touches = Joystick.ref:getTouches()
  --只允许一个触摸指令
  if (#touches > 1) then
    print("[State1.Player.Battle]only accecpt ONE parameter")
    Luah.output("[State1.Player.Battle]only accecpt ONE parameter"..#touches)
    return
  end
  for _,pad in ipairs(touches) do
    if (pad == CommandPad.attack1) then 
      local   gunPlayer = {count=-1,  sound=Folder.sound.."toygun.wav",rate=1.5, frame="bullet1.png",project={type=2,step=Vec2(3,3),target=22} }  
      player.ref:getWeapon():registerWeapon(gunPlayer)
      player.ref:getWeapon():shootAt(Vec2.ZERO)      
--    elseif (pad == "attack2") then
--      player.ref:getWeapon():shootAt(Vec2.ZERO)
    elseif (pad == CommandPad.attack2) then
      local   gunPlayer = {count=-1,  sound=Folder.sound.."shotgun.wav",rate=1.5, frame="bullet2.png",project={type=2,step=Vec2(3,3),target=22} }
      player.ref:getWeapon():registerWeapon(gunPlayer)
      player.ref:getWeapon():shootAt(Vec2.ZERO)      
    elseif (pad == CommandPad.hand) then
      player.ref:getWeapon():setFireOn()  
      Resh.playSound(Sound.question)
      player.ref:setAnimation(Animate.getToastDelay(RObjectText.littleHand.text,0.8))
      Role[RoleIndex.adboard].ref:registerText(RObjectText.adDrag)
      Role[24].fsm:changeState(State1.RObject.PickBat)
    elseif (pad == CommandPad.dodge) then
      Resh.playSound(Sound.question)
      player.ref:spawn(Vec2(15,10),Vec2.ZERO)
    elseif (pad == CommandPad.escape) then
      player.ref.moveStep = Vec2(0.4,0.4)
      player.ref:setDownward()
    end
    if (tostring(pad) ~= "") then
      MsgDispatcher.dispatch(1.5,player,Role[22],MessageType.attack, tostring(pad))
      State1.setPadVisible(false)
      return
    end

  end
end
State1.Player.Battle.onMessage = function(player,telegram)

  if player.ref:isDead() and player.isRecovered == nil then
    player.battleCommand = nil
    Role[22].battleCommand= nil
    player.isRecovered = true
    Ref.stage:prompt(PopupInfo:recover()) 
    return
  end

  if (telegram.message == MessageType.attack) then
    State1.setPadVisible(true)
  end

  if (telegram.message == MessageType.gotShot) then 
    local hp = 1
    local weaponRef = telegram.sender.ref:getOwner():getWeapon():getRef()
    local toast = 250
    if (weaponRef.frame == "poo.png") then
      hp = 250
    else
      hp = 2500
      toast =  "250 × 10"
    end
    player.ref:getWeapon():setFireOff()
    player.ref:gotHurt(hp,Sound.hurtPoo)
    player.ref:stopAnimation() 
    player.ref:setAnimation(Animate.getToastDelay("HP-"..toast,0.5))
    Joystick.ref:setString(CommandPad.barSteelball,RObjectText.barSteelball.text..tostring(player.ref.hp))
  end
  --重置玩家战斗指令
  player.battleCommand = nil
end
State1.Npc.WangbaBattle = State:new()
State1.Npc.WangbaBattle.enter = function(npc)
  npc.battleCommand = nil
end
State1.Npc.WangbaBattle.execute = function(npc)
  --如果王八死亡
  if (npc.ref:isDead()) then
    State1.setPadVisible(false)
    Resh.playSound(Sound.manDied)
    --开启最终对话
    CommonStates.Delay.fsm=Segment[4].fsm
    CommonStates.Delay.state=State1.Dialog.Final
    Segment[4].fsm:changeState(CommonStates.Delay)
    npc.fsm:clearState()
    return
  end

  local   gunWangba = {count=-1,  sound=Folder.sound.."poogun.wav",rate=0.3, frame="poo.png",project={type=2,step=Vec2(3,3),target=1,offset=Vec2(0,25)}}
  if (npc.battleCommand ~= nil) then
    npc.ref:getWeapon():registerWeapon(gunWangba)
  end
  if (npc.battleCommand == CommandPad.escape) then
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
  end
  if (npc.battleCommand == CommandPad.attack1) then

    npc.ref:getWeapon():shootAt(Vec2.ZERO)
    npc.battleCommand = nil
  elseif (npc.battleCommand == CommandPad.attack2) then
    gunWangba.frame="poo2.png"
    npc.ref:getWeapon():registerWeapon(gunWangba)
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
    npc.battleCommand = nil
  elseif (npc.battleCommand == CommandPad.hand) then
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
    npc.battleCommand = nil
  elseif (npc.battleCommand == CommandPad.dodge) then
    npc.ref:showHints(RObjectText.dickDodge)
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
  elseif (npc.battleCommand == CommandPad.escape) then
    npc.ref:showHints(RObjectText.dickEscape)
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
  end
end
State1.Npc.WangbaBattle.onMessage = function(npc,telegram)
  if (telegram.message == MessageType.attack) then
    npc.battleCommand = telegram.info 
    if (npc.battleCommand == CommandPad.attack1 or
      npc.battleCommand == CommandPad.attack2 or
      npc.battleCommand == CommandPad.hand) then
      MsgDispatcher.dispatch(1.5,npc,Role[1],MessageType.attack)
    end
  end

  if (telegram.message == MessageType.gotShot) then
    local hp = 1
    local weaponRef = telegram.sender.ref:getOwner():getWeapon():getRef()
    if (weaponRef.frame == "bullet1.png") then
      hp = 1
    else
      hp = 250 
    end  
    npc.ref:gotHurt(hp,Sound.wangbaHurt)
    npc.ref:stopAnimation()
    npc.ref:setAnimation(Animate.getToastDelay("HP-"..hp,0.2))
    npc.ref:getWeapon():shootAt(Vec2.ZERO)
    Joystick.ref:setString(CommandPad.barDick,RObjectText.barDick.text..tostring(npc.ref.hp))
  end
end

State1.RObject.PickBat = State:new()
State1.RObject.PickBat.enter = function(object)
  object.originPositionY = object.ref:getPosition().y
  object.ref:setAllowDragY(true)
end
State1.RObject.PickBat.execute = function(object)
  if (object.originPositionY - object.ref:getPosition().y  > 40 ) then
    object.ref.isVisible=false
    object.ref:setAllowDragY(false)
    object.fsm:clearState()
  end
end


State1.Dialog.Final= State:new() 
State1.Dialog.Final.enter = function(segment)
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end 
State1.Dialog.Final.execute = function(segment)

  DialogueSystem.update()
  if (DialogueSystem.isDone()) then
    segment.fsm:changeState(State1.Stage.Final) 

  end
end
State1.Dialog.Final.exit = function(segment)
  Role[RoleIndex.wangba].ref:closeDialog()
end

State1.Stage.Final= State:new() 
State1.Stage.Final.enter = function(segment)
  Ref.stage:promptAchieve(AchieveIndex.Beginning,PopupInfo.achieve(AchieveIndex.Beginning))
end 
State1.Stage.Final.execute = function(segment)
  --往后退
  Role[RoleIndex.wangba].ref:setUpward()
  if (Delay.isTimeUp(DelayIndex.stageEnd, 6)) then
    Ref.stage:prompt(PopupInfo.passed())
    segment.fsm:clearState()
  end
end
--候补
local bastardStart =  6
local bastardTag = 3
State1.Npc.BastardAndCrystal = State:new()
State1.Npc.BastardAndCrystal.execute = function(npc)
  Role[RoleIndex.crystal].ref:showHints({text="..................",font=Font.warn})
  --遍历每一个混蛋，
  if (Delay.isTimeUp(DelayIndex.spawn, 1.2)) then
    if (bastardTag <= RoleIndex.bastardEnd) then
      bastardTag = bastardTag + 1
    else
      bastardTag = RoleIndex.bastardStart
    end
    Delay.reset(DelayIndex.spawn)
  end
  --嘴里嘟囔
  if  npc.tag == bastardTag   then
    npc.ref:showHints({text="!@#!#$%^#$%^&**",font=Font.default})
  else
    npc.ref:closeDialog()
  end
end
State1.Npc.BastardAndCrystal.exit = function(npc)
  npc.ref:closeDialog()
  npc.ref:stopAnimation()
end
return State1