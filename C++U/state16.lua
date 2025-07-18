local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State = require("state")

local State16 = { Player={}, Npc={},RObject={}, ProjectTile={},Triggers={}, Dialogue={},Dick={},}

local grumble =  Hints.grumble[1]
local bastardTag = 2
local dickLaugh = math.random(#Hints.dickLaugh)
State16.initalize = function()
  dickLaugh = math.random(#Hints.dickLaugh)
end
local function laugh(npc)
  --嘲笑
  if (Role[1].ref:isDead() and npc.ref:isAlive()) then  
    if (npc.isLaughed == nil) then 
      npc.ref.onJump=false
      npc.isLaughed = true
      Resh.playSound(Sound.wangbaLaugh)
      npc.ref:getWeapon():unload()
     -- npc.ref:setAnimation( { type = Anistyle.jump, height=50,duration=1.5,limitedTimes=3})
    end
    npc.ref:stop()
    npc.ref:showHints(Hints.dickLaugh[dickLaugh])
    return true
  end

  return false
end

State16.Player.Routine = State:new()
State16.Player.Routine.execute = function(player)
  getStage():setCameraSlide(player.ref:getPosition(),CameraView.vertical) 
  PlayerStates.Routine.execute(player)
--    if (player.ref:isDead()) then
--      MsgDispatcher.dispatch(SEND_IMMEDIATELY,player,Role[RoleIndex.dick],MessageType.notice)
--      player.fsm:clearGlobal()
--   end
end
State16.Player.Routine.onMessage = function(player,telegram)

  if (  telegram.message == MessageType.gotShot ) then 
    local status = player.ref:gotHurt(1,Sound.hurtPoo)
  end
  if telegram.message == MessageType.hit then
    --注意是自己的对手
    if telegram.info ~= nil and telegram.info.tag == RoleIndex.zzi then
      player.ref:gotoHell(Sound.hitHead) 
      player.ref:setAnimation(Animate.rollOut)
      Ref.stage:promptAchieve(AchieveIndex.ZziSneak,PopupInfo.achieve(AchieveIndex.ZziSneak))
    else
      player.ref:gotHurt(1,Sound.hurtPoo)
    end
  end
--  if (BodyStatus.dead == status) then
--    MsgDispatcher.dispatch(SEND_IMMEDIATELY,player,Role[RoleIndex.dick],MessageType.notice)
--  end
end
--对话
State16.Dialogue.Dialog = State:new()
State16.Dialogue.Dialog.enter = function(segment)
  Role[1].ref:freeze()
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end
State16.Dialogue.Dialog.execute = function(segment)
  DialogueSystem.update()

  if (DialogueSystem.isDone()) then 
    Role[1].ref:unfreeze() 
--触发state
    Role[RoleIndex.dick].fsm:changeState(State16.Dick.Patrol) 
    Role[3].fsm:changeState(State16.Npc.Slime)
    Role[4].fsm:changeState(State16.Npc.Slime)
    Role[5].fsm:changeState(State16.Npc.Slime)
    Role[RoleIndex.zzi].fsm:changeState(State16.Npc.Zzi)
    Role[RoleIndex.missileRed].fsm:changeState(State16.RObject.Missile)
    Role[RoleIndex.missileBlue].fsm:changeState(State16.RObject.Missile)
    Checkpoint.id = Checkpoint.id + 1
    segment.fsm:clearState()
  end
end
State16.Dialogue.Dialog.exit = function(segment)
  DialogueSystem.close() 
  Resh.playMusic(Music.Boss)
end
--和汪思甜对话
State16.Dialogue.Zzi = State:new()
State16.Dialogue.Zzi.enter = function(segment)
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
  Role[1].ref:freeze()

end
State16.Dialogue.Zzi.execute = function(segment)
  DialogueSystem.update()

  if (DialogueSystem.isDone()) then 

    Role[1].ref:unfreeze()
    --  Role[RoleIndex.zzi].ref:recover()
    Role[RoleIndex.zzi].isExcepted = false
    Role[RoleIndex.zzi].ref:setFrame("zzi1.png")
    Role[RoleIndex.zzi].ref:stopAnimation()
    Role[RoleIndex.zzi].ref:setCheckTile(true)
    Role[RoleIndex.zzi].ref.allowGravity=true
    Role[RoleIndex.zzi].ref.isHarmful = true
    -- Role[RoleIndex.zzi].ref:setFrame(FrameStyle.idle,1,1)
    Role[RoleIndex.dick].fsm:changeState(State16.Dick.Idling)
    segment.fsm:clearState()
  end
end
State16.Dialogue.Zzi.exit = function(segment)
  DialogueSystem.close()
end
--张振爱
State16.Npc.Zzi = State:new()
State16.Npc.Zzi.execute = function(npc)
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if npc.isSaved == nil and
  Role[1].ref:isAlive() and
  Role[3].isDisposed  and
  Role[4].isDisposed  and
  Role[5].isDisposed  and
  bound.x > 9 and bound.y < 10 then
    npc.isSaved = true
    Role[RoleIndex.dick].allowRecover = false
    Segment[2].fsm:changeState(State16.Dialogue.Zzi)

  end
  --charge
  if npc.isSaved ~= nil and bound.x < 10 and bound.y < 10 then
    npc.ref:showHints(Hints.zziCharge)
    npc.ref:setBackward()
  end
  --死亡
  if (npc.ref:getBoundSelf().tilePosition.x < 2) then

    if npc.ref:isAlive() then
   Ref.stage:promptAchieve(AchieveIndex.ZziCharge,PopupInfo.achieve(AchieveIndex.ZziCharge))
 end 
 npc.ref:closeDialog()
    npc.isDisposed = true
  end
end
State16.Npc.Zzi.onMessage = function(npc,telegram)
  if (telegram.message == MessageType.gotShot) then
    npc.ref:gotoHell(Sound.manDied)  
    npc.ref:setFrame("zzi2.png")
    npc.ref:stop()
    npc.ref.isHarmful=false
    npc.ref:closeDialog()
    npc.fsm:clearState()
  end
end

--混蛋们
State16.Npc.Slime = State:new()
State16.Npc.Slime.execute = function(npc)

  if (npc.isDisposed) then
    return
  end 

  --遍历每一个混蛋，
  if (Delay.isTimeUp(DelayIndex.spawn, 1)) then
    if (bastardTag < RoleIndex.zzi -1) then
      bastardTag = bastardTag + 1
    else
      bastardTag = RoleIndex.agnail
    end

    grumble = Hints.grumble[math.random(1,#Hints.grumble)]
    Delay.reset(DelayIndex.spawn)
  end
  --嘴里嘟囔
  if (npc.tag == bastardTag and npc.ref:isAlive()) then
    npc.ref:showHints(grumble)
  else
    npc.ref:closeDialog()
  end
end
State16.Npc.Slime.exit = function(npc)
  npc.ref:closeDialog()
end
--碰一下消失
State16.Npc.Slime.onMessage = function(npc,telegram)
  npc.fsm:clearState() 
  npc.ref:setAnimation(Animate.fade) 
end
--迪克公共message
State16.Dick.onDickMessage = function(npc,telegram)
  if (telegram.message ==  MessageType.notice) then
    npc.fsm:clearState()
    laugh(npc)
    return
  end
  if (telegram.message == MessageType.gotShot) then
    local status = npc.ref:gotHurt(1,Sound.wangbaHurt)
    Joystick.ref:setString(CommandPad.barDick,RObjectText.dickHP..npc.ref.hp)

    if (BodyStatus.dead == status) then
      npc.fsm:changeState(State16.Dick.Recover)
    end

  end
end
-- 迪克 Idling
State16.Dick.Idling = State:new()
State16.Dick.Idling.enter = function(npc)

  npc.ref:spawn(Vec2(14,19),Vec2.ZERO)
  npc.ref.face = MovingDirection.toLeft
end
State16.Dick.Idling.execute = function(npc)
  if (laugh(npc)) then
    return
  end
  --清理巡逻边界,否则shoot  state会卡死
  npc.sectionRight= 0
  npc.ref:stop()
  npc.ref:getWeapon():setFireOff()
  local bound = Role[1].ref:getBoundSelf().tilePosition
  if bound.y > 16 and bound.y < 21 then
    npc.fsm:changeState(State16.Dick.Patrol)
  end
end
--1）巡逻
State16.Dick.Patrol = State:new()
State16.Dick.Patrol.execute = function(npc)
  if (laugh(npc)) then
    return
  end
  npc.ref:patrollingX(true)
  npc.updateTileSection()
 -- npc.shoot()
  if (npc.isReady()) then
    npc.fsm:changeState(State16.Dick.PatrolDodge)

  end
--  Luah.output("[State16.Patrol.Dick]getTileSection left:".. npc.sectionLeft.."right:"..npc.sectionRight)

end
State16.Dick.Patrol.onMessage = State16.Dick.onDickMessage
--2）巡逻和跳跃
State16.Dick.PatrolDodge = State:new()
State16.Dick.PatrolDodge.execute = function(npc) 
  if (laugh(npc)) then
    return
  end

  npc.ref:patrollingX(true)
  npc.updateTileSection()
 -- npc.shoot()
  npc.ref:getWeapon():setFireOn()
  local boundPlayer =  Role[1].ref:getBoundSelf().tilePosition
  local boundNpc = npc.ref:getBoundSelf().tilePosition
  npc.ref.onJump=math.abs(boundPlayer.x - boundNpc.x) == 2

  if (npc.isReady()) then
    npc.fsm:changeState(State16.Dick.Shoot)
  end

end

State16.Dick.PatrolDodge.onMessage = State16.Dick.onDickMessage
--3)发射子弹 以及炮弹
State16.Dick.Shoot = State:new()
State16.Dick.Shoot.enter = function(npc)
  Delay.reset(DelayIndex.common)
  npc.ref:stop()
end
State16.Dick.Shoot.execute = function(npc) 
  if (laugh(npc)) then
    return
  end

  npc.ref:getWeapon():shootAt(Vec2(0,0))

  if Delay.isTimeUp(DelayIndex.common,4) then
    npc.fsm:changeState(State16.Dick.Patrol) 
  end
end
State16.Dick.Shoot.exit = function(npc)
  if (npc.ref.hp <= npc.hpMax * 0.5) then
    --发射炮弹
    Resh.playSound(Sound.missile)
    Role[RoleIndex.missileRed].direction = MovingDirection.toLeft
    Role[RoleIndex.missileBlue].direction = MovingDirection.toLeft
  end
end
State16.Dick.Shoot.onMessage   = function(npc, telegram)
  if (telegram.message == MessageType.gotShot) then
    Resh.playSound(Sound.blocked2)
  end
end
--4)回血
State16.Dick.Recover = State:new()
State16.Dick.Recover.enter = function(npc)
  Delay.reset(DelayIndex.recover)
  Resh.playSound(Sound.manDied)
  npc.ref:stop()
  --不能回血则结束
  if (not npc.allowRecover) then
    npc.ref.isHarmful = false
    npc.ref:spawn(Vec2(14,19),Vec2.ZERO)
    npc.ref.face = MovingDirection.toLeft
    Segment[3].fsm:changeState(State16.Dialogue.Delay)
    npc.fsm:clearState()
  end
end
State16.Dick.Recover.execute = function(npc) 
  npc.ref:stop()
  npc.ref:showHints(Hints.dickRecovering)
  if Delay.isTimeUp(DelayIndex.recover,8) then
    npc.ref.hp = npc.hpMax
    npc.ref:recover()
    Resh.playSound(Sound.wangbaLaugh)
    Ref.stage:promptAchieve(AchieveIndex.DickRecover,PopupInfo.achieve(AchieveIndex.DickRecover))
    Joystick.ref:setString(CommandPad.barDick,RObjectText.dickHP..npc.ref.hp)
    npc.fsm:changeState(State16.Dick.Patrol)
  end
end
State16.Dick.Recover.onMessage   = function(npc, telegram)
  if (telegram.message == MessageType.gotShot) then
    Resh.playSound(Sound.blocked2)
  end
end
State16.Dick.Recover.exit = function(npc)
  npc.ref:closeDialog()
end
--[延迟]迪克对话
State16.Dialogue.Delay = State:new()
State16.Dialogue.Delay.enter = function(segment)
  Delay.reset(DelayIndex.common) 
  Role[1].ref:freeze()
  Role[1].ref:spawn(Vec2(4,19),Vec2.ZERO)
   Role[1].ref.face = MovingDirection.toRight
  Role[RoleIndex.dick].ref:respawn()
end
State16.Dialogue.Delay.execute = function(segment) 
  Role[1].ref.face = MovingDirection.toRight
  Role[1].ref.isFlippedX = false
  Role[RoleIndex.dick].ref.face = MovingDirection.toLeft
    Role[RoleIndex.dick].ref.onJump =false

  Role[RoleIndex.dick].ref.isFlippedX = true
  if not Delay.isTimeUp(DelayIndex.common, 3) then
    return
  end
  segment.fsm:changeState(State16.Dialogue.Dick)
end
--迪克对话
State16.Dialogue.Dick = State:new()
State16.Dialogue.Dick.enter = function(segment)
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
end
State16.Dialogue.Dick.execute = function(segment)

  DialogueSystem.update()
 
  if (DialogueSystem.isDone()) then 
    Role[1].ref:unfreeze()
    Role[RoleIndex.dick].ref:setAnimation(Animate.fade)
    Ref.stage:prompt(PopupInfo.passed())
    segment.fsm:clearState()
  end
end
State16.Dialogue.Dick.exit = function(segment)
  DialogueSystem.close()
end

State16.RObject.Missile =  State:new()  
State16.RObject.Missile.execute = function(object) 
  if (object.direction == MovingDirection.stayStill) then
    return
  end
  object.ref:setDirection(object.direction)
  if  object.ref:getPositionX() < 0  then
    object.ref:stop()
    object.direction = MovingDirection.stayStill
    object.ref:resetVelocity()
    object.ref:respawn()
  end
end
State16.RObject.Missile.onMessage = function(object,telegram)
  if  telegram.message == MessageType.collider and telegram.sender.ref:getCollidedDirection() == CollisionDirection.atLeft   then
    Role[1].ref:gotHurt(1,Sound.manHurt1)
  end
end
return State16