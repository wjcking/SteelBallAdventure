local Telegram = require("telegram")
local PlayerStates = require("playerstates")
local State = require("state")
local RObjectStates = require("robjectstates")
local CommonStates = require("commonstates")
local State17 = { Player={}, Npc={},RObject={}, ProjectTile={},Wangba={},Dialog={}, Trigger={},Stage={},Dialogue={},}
State17.cheers = function()
  for _,each in ipairs(Role) do
    if (each.tag ~= 1 and each.face ~= nil) then
      each.fsm:changeState(State17.Npc.Bastard)
    end
  end
end
State17.quiet = function()
  for _,each in ipairs(Role) do
    if (each.tag ~= 1 and each.face ~= nil) then
      each.ref:closeDialog()
      each.fsm:clearState()
    end
  end
end
--玩家钢蛋
State17.Player.Routine = State:new()
State17.Player.Routine.execute = function(player)
  getStage():setCameraSlide(player.ref:getPosition(),CameraView.vertical)
  --Role[1].ref:limiteScreenBorder(BorderLimited.limiteVertical)
  PlayerStates.Routine.execute(player)

  local bound = player.ref:getBoundSelf().tilePosition

  if player.isReachedCrystal == nil and (bound.x == 8 or bound.x == 9) and bound.y == 19 then
    Segment[1].fsm:changeState(State17.Dialogue.Dialog)
    player.isReachedCrystal = true
  end
  --过关
    if   (bound.x == 8 or bound.x == 9) and bound.y == 4 then
      player.ref:freeze()
    Ref.stage:prompt(PopupInfo.passed())
    player.fsm:clearGlobal()
    player.fsm:clearState()   
  end
end
State17.Player.Routine.onMessage = function(player, telegram)

end

--对话
State17.Dialogue.Dialog = State:new()
State17.Dialogue.Dialog.enter = function(segment)
  Role[1].ref:freeze()
  Role[1].ref.face = MovingDirection.toTop
  local starCount = getStarCount()
  if starCount >= 12 then
    segment.rangeStart = 11
    segment.rangeEnd= 15
  end
  DialogueSystem.setIndex(segment.rangeStart,segment.rangeEnd)
  State17.quiet()
end
State17.Dialogue.Dialog.execute = function(segment)
  DialogueSystem.update()

  if (DialogueSystem.isDone()) then 
    Role[1].ref:unfreeze() 
    segment.fsm:clearState()
     Role[RoleIndex.crystal].ref:stopAnimation()
    Role[RoleIndex.crystal].ref:setAnimation(Animate.fadeCrystal)
    State17.cheers()
  end
end
State17.Dialogue.Dialog.exit = function(segment)
  DialogueSystem.close()
end
--玩家钢蛋
State17.Npc.Routine = State:new()
State17.Npc.Routine.execute = function(npc)

end
local bastardStart =  3
local grumbleID = 1
local bastardTag = 3
local function cheers(npc)
  if (math.random(1,10) > 5) then
    npc.ref:getWeapon():setFireOn()
  else
    npc.ref:getWeapon():setFireOff()
  end  
end
State17.Npc.Bastard = State:new()
State17.Npc.Bastard.execute = function(npc)
  --遍历每一个混蛋，
  if (Delay.isTimeUp(DelayIndex.spawn, 2)) then
    if (bastardTag <= RoleIndex.bastardEnd) then
      bastardTag = bastardTag + 1
    else
      bastardTag = RoleIndex.bastardStart
    end

    grumbleID = math.random(1,#Hints.grumble)
    Delay.reset(DelayIndex.spawn)
  end
  --嘴里嘟囔
  if (npc.tag == bastardTag ) then
    npc.ref:showHints(Hints.grumble[grumbleID])
  else
    npc.ref:closeDialog()
  end


  local bound = Role[1].ref:getBoundSelf().tilePosition
  if (bound.x == 8 or bound.x == 9) and bound.y >= 19 and bound.y <= 21  then
    if (npc.standPosition ~= nil ) then 
      npc.ref.face = MovingDirection.toRight
      npc.ref.isFlippedX =false
    else 
      npc.ref.face = MovingDirection.toLeft
      npc.ref.isFlippedX = true
    end
  elseif (bound.y <=18) then
    npc.ref.face=MovingDirection.toTop
    cheers(npc)
  elseif (bound.y >= 22) then
    npc.ref.face= MovingDirection.toBottom 
    cheers(npc)
  end
end
return State17