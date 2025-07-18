local State = {}

function State:new(name, enter, execute, exit, msg)
	local state = {}
	state.name = name 
	state.enter = enter
	state.execute = execute
	state.exit = exit
  state.onMessage = msg
  setmetatable(state, {__index = State})  
	return state
end
return State