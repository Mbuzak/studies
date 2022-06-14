require 'timeout'

class Game
	def initialize(level, rounds, range)
		@level = level
		@result = []
		@rounds = rounds
		@round = 0
		@range = range
		@difficulty = {"łatwy" => [4, 8],
					   "normalny" => [2, 4],
					   "trudny" => [1, 2]}
	end
	
	def random_number
		return rand(@range[0]..@range[1])
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
		@number = random_number
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
				puts 'Niepoprawna wartość'
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
			
			if counter >= get_max_choice
				puts
				puts "Przegrana"
				puts "Naciśnij ENTER aby wrócić do menu..."
				gets
				return -1
			end
		end
	end
	
	def game
		puts "Zakres liczb <#{@range[0]}, #{@range[1]}>"
		puts "Ilość prób na rundę: #{get_max_choice}"
		puts 'Aby zakończyć napisz "koniec"'
		puts
		for i in 1..@rounds
			puts
			puts "Runda #{i} | #{get_max_time} sekund"
			puts "Aby zacząć, naciśnij ENTER"
			gets
			begin
				status = Timeout::timeout(get_max_time) do
					res = find
					if res != -1
						@result.append(res)
						@round += 1
					else
						return -1
					end
				end
				rescue Timeout::Error
				puts
				puts "Przegrana"
				puts "Naciśnij ENTER aby wrócić do menu..."
				gets
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
		return (Math.log((@range[1] - @range[0] + 1), 2).ceil * @difficulty[@level][0]).to_i
	end

=begin
Max time caclutate for "get_max_time" function which is implemented by myself.

So for example when number is generate in range 1-1024, then log(1024, 2) = 10. So 10 is best possible result(user choices counter) if user use math while playing game :).

Then time levels will be for (1st, last round):
	- easy (10 * 16 = 160s, 10 * 8 = 80s)
	- normal (10 * 8 = 80s, 10 * 4 = 40s)
	- hard (10 * 4 = 40s, 10 * 2 = 20s)
=end
	
	def get_max_time
		min_choice = Math.log((@range[1] - @range[0] + 1), 2).ceil
		last_round_time = min_choice * @difficulty[@level][1]
		first_round_time = last_round_time * 2
		
		diff = first_round_time - last_round_time
		level_time = (diff * (@round.to_f / (@rounds.to_f - 1))).ceil
		
		return (first_round_time - level_time).to_i
	end
end

# --- TESTS ---
def tests
	game = Game.new("trudny", 2, [1, 2])
	game.game
	puts game.results.sum
	puts game.count_rounds
end

# tests
