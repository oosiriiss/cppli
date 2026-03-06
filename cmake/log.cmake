# Logs a message.
# argumentes are separted by a space
function(log)
   set(msg "CPPLI ::")

   foreach(PART IN ITEMS ${ARGV})
      set(msg "${msg} ${PART}")
   endforeach()

   message(${msg})
endfunction()
