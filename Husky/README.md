Husky
=====

Platform specific additions to Moai/Sledge.

Huskies
=======

 * Dummy
   * Only runs locally, used to test the implementation.
   * All Achievements will succeed, except for one called "Failed Achievement" which will return a failure.


Huskies that will be implemented
=====================

 * iOS
   * Game Centre Achievements and Leaderboards
 * Google Play
   * Achievements and Leaderboards
 * Mac OS X
   * Game Centre Achievements and Leaderboards


Usage
====

Sledge will, by default, link to MOAIHusky and handle loading for you. iOS and Android will be linked statically with their platform specific husky. The desktop platforms will search for dynamic libraries located in the same directory as the moai-sdl executable and load the first one by default.

In Lua land you then have access to the MOAIHusky singleton which will have the following functions:

* MOAIHusky:getAvailable()
* MOAIHusky:getCurrent()
* MOAIHusky:achievementSetCallback(<callback function>)
* MOAIHusky:achievementSet(<string>)

Example code below:

<pre>
list = MOAIHusky:getAvailable()

print("Available Huskies:")
for index, name in pairs(list) do
 print("  "..name)
end

function achievecallback(name, success)
  if (success) then
    print("Achievmeent "..name.." set");
  else
    print("Achievmeent "..name.." DIDNT WORK");
  end
end

print("Current Husky"..MOAIHusky:getCurrent())

MOAIHusky:achievementSetCallback(achievecallback);
MOAIHusky:achievementSet("Failed Achievement");
</pre>