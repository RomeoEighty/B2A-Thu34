puts "P1"
puts "1024 1024"
for i in 1..1024
    print "0"
    for j in 2..512
        print " 0"
    end
    for j in 513..515
        print " 1"
    end
    for j in 516..1024
        print " 0"
    end
    print "\n"
end
