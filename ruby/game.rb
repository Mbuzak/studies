class Game
	def initialize(level, rounds, range)
		@level = level
		@result = []
		@rounds = rounds
		@round = 0
		@range = range
		@difficulty = {"łatwy" => [4, 10],
					   "normalny" => [2, 5],
					   "trudny" => [1, 3]}
	end
	
	def random_number
		@number = rand(@range[0]..@range[1])
	end
	
	def end_confirm
		print "Czy na pewno chcesz zakończyć? t/n "
		while true
			decision = gets.chomp
			if decision == "t"
				return 0
			elsif decision == "n"
				return 1
			else
				next
			end
		end
	end
	
	def compare(choice)
		if choice < @number
			puts "Więcej"
		elsif choice > @number
			puts "Mniej"
		else
			puts "OK"
			return 1
		end
		
		return 0
	end
	
	def find
		random_number
		choice = 0
		counter = 0
		while @number != choice
			print "> "
			choice = gets.chomp
			
			if choice == 'koniec'
				if end_confirm == 0
					return -1
				else
					next
				end
			end
			
			begin
				choice = Integer(choice)
			rescue ArgumentError
				puts('Niepoprawna wartość')
				next
			end
			
			choice = choice.to_i
		
			if choice < @range[0] or choice > @range[1]
				puts('Liczba poza zakresem')
				next
			end
			
			#compare(choice)
			counter += 1
			if compare(choice) == 1
				return counter
			end
			
			if counter > get_max_choice
				puts("Przegrana")
				return -1
			end
		end
	end
	
	def game
		puts "Zakres liczb <#{@range[0]}, #{@range[1]})"
		puts
		for i in 1..@rounds
			puts "Runda #{i}"
			res = find
			if res != -1
				@result.append(res)
				@round += 1
			else
				return -1
			end
		end
		
		puts "Zwycięstwo"
	end
	
	def results
		return @result
	end
	
	def count_rounds
		return @round
	end
	
	def get_max_choice
		return (Math.log((@range[1] - @range[0] + 1), 2) * @difficulty[@level][0]).to_i
	end
end

# --- TESTS ---
def tests()
	game = Game.new("trudny", 2, [1, 2])
	#puts game.get_max_choice
	game.game
	#puts game.get_max_choice
	puts game.results.sum
	puts game.count_rounds
end

#tests()