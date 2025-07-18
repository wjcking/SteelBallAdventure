local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State = require("state")

local State15 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},}


State15.loadPhase3 = function()
  --改变cammera
  if (  Role[1].allowThrough) then
    return
  end
  Role[RoleIndex.brick1].isDisposed = true
  Role[RoleIndex.brick2].isDisposed = true
  Role[RoleIndex.switchPhase1Left].isDisposed = true
  Role[RoleIndex.switchPhase1Right ].isDisposed = true
  Role[RoleIndex.spikeTop].isDisposed = true
  Role[RoleIndex.switchPhase2Left].isDisposed = true
  Role[RoleIndex.switchPhase2Right].isDisposed = true
  Role[RoleIndex.switchPhase2Middle].isDisposed = true
  Role[RoleIndex.slimePhase2].isDisposed = true
  Role[RoleIndex.questionLeft].isDisposed = true
  Role[RoleIndex.slimePhase3].isDisposed = true

  Role[RoleIndex.magicBrick].allowTaken = true
  Role[RoleIndex.magicBrick].ref:spawn(Vec2(11,21),Vec2.ZERO)  
  Role[1].ref:setZOrder(ZOrder.role)

  Role[1].allowThrough = true
  --如果阶段1，2 已经过了，否则不记录
  if (Checkpoint.id < 3) then
    Checkpoint.setPtc(Vec2(7,39))
  end
  --星星
  createStar(10,Vec2(16,21),ZOrder.robject)
  Role[RoleIndex.board].ref:spawn(Vec2(8,6),Vec2(10,15))
  Role[RoleIndex.board].ref:registerText(RObjectText.board4)
  Role[RoleIndex.questionRight].ref:registerSpring( {jumpForce=Vec2(0,18)})
  Role[RoleIndex.questionRight].ref:setFrame("spring_red1.png")
  Role[RoleIndex.questionRight].ref:spawn(Vec2(8,42),Vec2(20,0))
--移除倒刺 
   getStage():removeTileRange(Vec2(1,40),Vec2(16,41))
  --打开通路直冲天
  getStage():removeObjectRange(Vec2(8,12),Vec2(9,42))
  getStage():removeTileRange(Vec2(8,12),Vec2(9,42))
  Role[RoleIndex.questionRight].fsm:changeState(State15.RObject.Spring)
  Role[RoleIndex.door].fsm:changeState(State15.RObject.Door)

end
State15.Player.Routine = State:new()
State15.Player.Routine.execute = function(player) 
  PlayerStates.Routine.execute(player)
  if  player.ref:isDead() then
    return
  end
  Joystick.ref:setVisible(CommandPad.drop, player.ref:isCarrying())
  if (not player.allowThrough) then
    getStage():setCameraSlide(player.ref:getPosition(),CameraView.vertical) 
  else
    getStage():setCameraCenter(player.ref,CameraView.vertical,true)
  end
  player.ref:checkSpike()


  if (Checkpoint.id == 0 and  Role[1].ref:getBoundSelf().tilePosition.y >= 24) then

    Checkpoint.setPtc(Vec2(7,31))
    player.ref:setZOrder(ZOrder.role)
    Role[RoleIndex.board].ref:registerText(RObjectText.board2)
    Role[RoleIndex.board].ref:spawn(Vec2(8,26),Vec2(10,15))
    Role[RoleIndex.board].ref:setAllowTouch(true)
    Role[RoleIndex.board].ref:setAllowDragX(true)
    Role[RoleIndex.switchPhase2Left].fsm:changeState(State15.RObject.Switch2)
    Role[RoleIndex.switchPhase2Right].fsm:changeState(State15.RObject.Switch2)
    Role[RoleIndex.switchPhase2Middle].fsm:changeState(State15.RObject.Switch2)
    Role[RoleIndex.slimePhase2].fsm:changeState(State15.Npc.Slime)
  end
end
State15.Triggers.Trap = State:new()
State15.Triggers.Trap.execute = function(trigger)

  local bound = Role[1].ref:getBoundSelf().tilePosition
  if  trigger.isPhase1TrapDone == nil  and bound.y == 22 and (bound.x==11 or bound.x==12) then
    getStage():setGidRange(Gid.spike,Vec2(11,22),Vec2(12,23))
    trigger.isPhase1TrapDone = true
    trigger.fsm:clearState()
  end
end
State15.RObject.Switch = State:new()
State15.RObject.Switch.execute = function(object)
  object.getOpponents(object,function(range,opponent)

      if (object.type == RoleType.npc or range < RangeType.rangeThreatened) then 
        return
      end

      object.switchResult = object.ref:switchFrame(opponent.ref)
      --左侧开关打开，出发陷阱 and  Triggers[1].isPhase1TrapOn ~=nil
      if  object.tag == RoleIndex.switchPhase1Left and object.switchResult:getIndex() > 1 and object.isPhase1TrapOn then

        Role[RoleIndex.brick1].ref.isHarmful=true
        Role[RoleIndex.brick2].ref.isHarmful=true
        Role[RoleIndex.brick1].ref.allowGravity=true
        Role[RoleIndex.brick2].ref.allowGravity=true
        Role[RoleIndex.brick1].ref.isSolid = false
        Role[RoleIndex.brick2].ref.isSolid = false
        getStage():setGidRange(Gid.brickRed,Vec2(3,20),Vec2(3,21))
        Resh.playSound(Sound.fall)
        object.fsm:clearState()  
      elseif  object.tag == RoleIndex.switchPhase1Right and 
      object.switchResult:getIndex() > 1  and
      Role[RoleIndex.switchPhase1Left].switchResult ~= nil and
      Role[RoleIndex.switchPhase1Left].switchResult:getIndex() > 1   then
        getStage():removeTileRange(Vec2(8,22),Vec2(9,23))
        getStage():removeTileRange(Vec2(11,22),Vec2(12,23))
        Triggers[1].fsm:changeState(State15.Triggers.Trap)
        Resh.playSound(Sound.enigma)
        object.fsm:clearState()  
        Role[RoleIndex.switchPhase1Left].fsm:clearState()  
        Role[RoleIndex.board].ref:registerText(RObjectText.board1Done)
      end
    end)
end
State15.RObject.Switch2 = State:new()
State15.RObject.Switch2.execute = function(object)
  object.getOpponents(object,function(range,opponent)

      if (object.type == RoleType.npc or range < RangeType.rangeThreatened) then 
        return
      end

      object.switchResult = object.ref:switchFrame(opponent.ref)

    end)
  if (  Role[RoleIndex.switchPhase2Left].switchResult:getIndex() > 1 and
    Role[RoleIndex.switchPhase2Right].switchResult:getIndex() > 1 and
    Role[RoleIndex.switchPhase2Middle].switchResult:getIndex() > 1) then
    Ref.stage:promptAchieve(AchieveIndex.SlimeSwitch,PopupInfo.achieve(AchieveIndex.SlimeSwitch)) 
    Role[RoleIndex.slimePhase2].isDisposed = true 
    Role[RoleIndex.switchPhase2Left].isDisposed = true 
    Role[RoleIndex.switchPhase2Right].isDisposed = true 
    Role[RoleIndex.switchPhase2Middle].isDisposed = true 
    --存档
    Checkpoint.setPtc(Vec2(8,39))
    getStage():removeTileRange(Vec2(8,32),Vec2(9,34))
    Resh.playSound(Sound.enigma)

    --第三个阶段
    Role[RoleIndex.board].ref:registerText(RObjectText.board3)
    Role[RoleIndex.board].ref:spawn(Vec2(8,36),Vec2(10,15))
    Role[RoleIndex.board].ref:setAllowDragX(false)

    Role[RoleIndex.questionLeft].fsm:changeState(State15.RObject.Question)
    Role[RoleIndex.questionRight].fsm:changeState(State15.RObject.Question) 
    Role[RoleIndex.slimePhase3].fsm:changeState(State15.Npc.Slime)
  end
end
State15.RObject.Switch.onMessage = function(object,telegram)
  if (telegram.message == MessageType.collider) then
    --右侧开关提前拉下，陷阱开启
    if  object.tag == RoleIndex.switchPhase1Right and 
    ( Role[RoleIndex.switchPhase1Left].switchResult== nil or
      Role[RoleIndex.switchPhase1Left].switchResult:getIndex() <= 1 ) then
      Role[RoleIndex.switchPhase1Left].isPhase1TrapOn = true
    end
  end
end
--推砖头
State15.RObject.MagicBrick = State:new()
State15.RObject.MagicBrick.execute = function(object)

end
State15.RObject.MagicBrick.onMessage = function(object,telegram)
  if (telegram.message == MessageType.pushed) then
    object.ref:gotPushed(telegram.sender.ref)
  end
end
--推砖头掉
State15.RObject.Spikes = State:new()
State15.RObject.Spikes.execute = function(object)

  if (object.tag == RoleIndex.spikeTop) then
    local bound = Role[RoleIndex.magicBrick].ref:getBoundSelf().tilePosition
    if (bound.x == 7 and bound.y == 21) then
      object.ref.allowGravity = true
      object.fsm:changeState(CommonStates.OutForDisposal)
    end
  else -- 就一个后加的
--    local bound = Role[1].ref:getBoundSelf().tilePosition
--    if  bound.x == 14 then
--      object.ref.isHarmful = true
--      object.ref.isVisible = true
--      object.ref.allowGravity = true
--      object.fsm:changeState(CommonStates.OutForDisposal)
--    end
  end
end
--对话框
State15.RObject.Board = State:new()
State15.RObject.Board.execute = function(object)
  if (Role[1].ref:isDead()) then
    object.ref:registerText(RObjectText.boardDead)
    object.fsm:clearState()
  end
end
State15.RObject.Board.onMessage = function(object,telegram)
  if (telegram.message == MessageType.gotShot) then
    object.ref:registerText(RObjectText.gotShot(Role[1].ref:getWeapon():getSlugLeft()))
  end
end
--问号
State15.RObject.Question = State:new()
State15.RObject.Question.execute = function(object)

  if (object.tag == RoleIndex.questionLeft) then
    local bound = object.ref:getBoundSelf().tilePosition
    if (bound.x == 13 and bound.y == 36) then
      Role[RoleIndex.questionRight].frame = "question_green.png"
      Role[RoleIndex.questionRight].ref:setFrame("question_green.png")
      object.fsm:clearState()
      object.isDisposed = true
      Resh.playSound(Sound.object)
    end
--**********************最后阶段*************************
  elseif object.tag == RoleIndex.questionRight and (object.frame=="bomb1.png" ) then
    if (object.ref:isDead()) then

      object.ref:setAnimation(Animate.explosion)

      local bound = object.ref:getBoundSelf().tilePosition
      --周围爆炸
      getStage():removeTileRange(Vec2(bound.x-1,40),Vec2(bound.x + 1,40))
      object.ref:registerSpring( {jumpForce=Vec2(0,18)})
      object.ref:setFrame("spring_red1.png")
      object.ref:spawn(Vec2(8,42),Vec2(20,0)) 
      object.fsm:changeState(State15.RObject.Spring)

      return
    end
    object.getTarget(object,Role[RoleIndex.slimePhase3],function(range,opponent)
        if  range < RangeType.rangeThreatened or opponent.ref.moveStep.x < 6  then 
          return
        end

        local isCollided = object.ref:checkObjectCollision(opponent.ref,false)
        if (isCollided) then
          object.ref:gotHurt(1,Sound.blocked1)
          object.ref:setAnimation(Animate.getToastHurt("HP:"..object.ref.hp))
        end

      end) 
  end
end
State15.RObject.Question.onMessage = function(object,telegram)
  if (telegram.message == MessageType.collider) then
    --如果发出者是player
    if (telegram.sender.tag == 1) then

      if (object.frame == "question_green.png") then
        getStage():removeTileRange(Vec2(1,37),Vec2(16,41))
        object.isDisposed = true
        Resh.playSound(Sound.object)
        Ref.stage:promptAchieve(AchieveIndex.FallTogether,PopupInfo.achieve(AchieveIndex.FallTogether)) 
      elseif(object.tag == RoleIndex.questionLeft ) then
        local gunObject= {  count=50,  sound=Folder.sound.."toygun.wav",rate=1, frame="bullet1.png",project={type=ProjectType.object, allowGravity=false,step=Vec2(6,6)}}
        Role[1].ref:getWeapon():registerWeapon(gunObject)
        Joystick.ref:setVisible(CommandPad.fire,true) 
        object.isDisposed = true
        Resh.playSound(Sound.object)
      elseif(object.tag == RoleIndex.questionRight and object.frame == "question_blue.png") then
        Role[RoleIndex.slimePhase3].ref.moveStep= Vec2(10,10)
        Role[RoleIndex.slimePhase3].allowPatroll = true
        object.isDisposed = true
        Resh.playSound(Sound.object)
      end
    end

  end
--打一枪，变成可推
  if (telegram.message == MessageType.gotShot) then

    object.ref.isSolid = true
    object.ref.allowGravity=true
    object.ref:allowPush( CollisionDirection.intersected)
    object.frame = "bomb1.png"
    object.ref:setFrame(object.frame)
  end
  if (telegram.message == MessageType.pushed) then
    object.ref:gotPushed(telegram.sender.ref)
  end
end
State15.Npc.Slime = State:new()
State15.Npc.Slime.execute = function(npc)
  if (Role[1].ref:isDead()) then
    npc.ref:setFrameIndex(FrameStyle.walkX,4,4)
    npc.ref:setFrameIndex(FrameStyle.idle,4,4)
    npc.ref:stop()
    npc.fsm:clearState()
  end
  if (npc.tag == RoleIndex.slimePhase2) then
    local bound = npc.ref:getBoundSelf().tilePosition
    npc.ref.onJump=Role[1].ref.onJump
    npc.ref:patrollingX(not (bound.y == 28 and bound.x > 5 and bound.x < 12))
  elseif (npc.tag == RoleIndex.slimePhase3) then
    npc.ref:checkSpike()
    if (npc.allowPatroll) then
      npc.ref:patrollingX(true) 
    end
  end
end
State15.Npc.Slime.onMessage = function(npc,telegram)
  if (telegram.message == MessageType.gotShot) then
    npc.allowPatroll = true
    npc.ref.moveStep= Vec2(npc.ref.moveStep.x + 1,npc.ref.moveStep.y+1)
    if (npc.ref.moveStep.x > 5) then
      npc.ref:setFrameIndex(FrameStyle.walkX,2,2)
    end

    if  npc.ref.moveStep.x > 10 then
      Ref.stage:promptAchieve(AchieveIndex.CrazySlime,PopupInfo.achieve(AchieveIndex.CrazySlime))
    end
  end
end
--final , 
State15.RObject.Door = State:new()
State15.RObject.Door.enter = function(object)
  object.ref.moveStep = Role[1].ref.moveStep
end
State15.RObject.Door.execute = function(object) 
  if (Role[1].ref:isDead()) then
    object.fsm:clearState()
    return
  end

  if (object.frame == "door2.png") then
    object.ref:stop()
    return
  end

  object.getTarget(object,Role[RoleIndex.magicBrick],function(range,opponent)
      --如果玩家带着砖头，则退出
      if  range < RangeType.rangeThreatened  then 
        return
      end

      local isCollided =   object.ref:checkObjectCollision(opponent.ref,false)
      if (isCollided and not Role[1].ref:isCarrying()) then
        object.ref:checkObjectSide()
        object.ref:checkFollowing()
        object.frame = "door2.png"
        object.ref:setFrame( object.frame)
        Role[RoleIndex.board].ref:registerText(RObjectText.door)
      end

    end) 

  local boundSelf = object.ref:getBoundSelf().tilePosition
  local boundPlayer = Role[1].ref:getBoundSelf().tilePosition
  if (boundPlayer.y > boundSelf.y) then
    return
  end
  object.ref:showHints(Hints.door)
  local rangeX = math.abs(boundPlayer.x - boundSelf.x)
  if rangeX == 2 and Role[1].ref:isMoving() and Role[1].ref.face == MovingDirection.toLeft then
    object.ref:setBackward()
  elseif   rangeX == 2  and Role[1].ref:isMoving() and Role[1].ref.face == MovingDirection.toRight then
    object.ref:setForward()

  end 
end

State15.RObject.Door.onMessage = function(object,telegram)
  if  telegram.message == MessageType.collider and  object.frame == "door2.png"  then
    Role[1].ref:freeze()
    Ref.stage:prompt(PopupInfo.passed())
    object.fsm:clearState()
  end
end
State15.RObject.Spring = State:new()
State15.RObject.Spring.onMessage = function(object,telegram)
  --and object.ref:getCollidedDirection() == CollisionDirection.atTop
  if  telegram.message == MessageType.collider and object.ref:bounce(Role[1].ref).isBounced  then
    --载入最后阶段 
    object.ref:getWeapon():unload()
    Joystick.ref:setVisible(CommandPad.fire,false)
    State15.loadPhase3()
    --门复位
    Role[RoleIndex.door].ref:stop()
    Role[RoleIndex.door].ref:closeDialog()
    Role[RoleIndex.door].ref:spawn(Vec2(8,8),Vec2(20,0))
    Resh.playSound(Sound.spring)
    if (object.frame == "spring_red1.png") then
      object.frame = "spring_red2.png"
    else
      object.frame = "spring_red1.png"
    end
    object.ref:setFrame(object.frame)
  end
end
return State15