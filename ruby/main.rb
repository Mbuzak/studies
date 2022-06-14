local_dir = File.expand_path('../', __FILE__)
$LOAD_PATH.unshift(local_dir)

require "score.rb"
require "game.rb"


$range = [1, 128]
$difficulty_level = "normalny"
$levels = {"1" => "łatwy", "2" => "normalny" ,"3" => "trudny"}

$scoreboard = Scoreboard.new(10)
$rounds = 7

def clear_screen
	if ENV['OS'] =~ /Windows_NT/
		system("cls")
	else
		system("clear")
	end
end


def set_user
	print("Podaj swoje imię: ")
	user = gets.chomp
	
	if user.length > 0
		return user
	else
		return "anonim"
	end
end


def save
	File.open('scoreboard.txt', 'w') { |file| file.write($scoreboard.to_txt) }
end

def load
	file = File.open('scoreboard.txt') { |file| file.readlines }
	for line in file
		line = line.chomp
		split = line.split(",")
		score = Score.new(split[0], split[1].to_i, split[2].to_i, split[3])
		$scoreboard.append(score)
	end
end


class Menu
	def show_menu
		puts
		puts("\tGra w zgadywanie")
		puts
		puts("  (1) Nowa gra")
		puts("  (2) Poziom trudności: #{$difficulty_level}")
		puts("  (3) Wyniki")
		puts("  (4) Wyjście")
		puts
	end
	
	def show_results
		clear_screen
		puts $scoreboard
		puts
		puts "Naciśniej ENTER aby wrócić do menu..."
		gets
		clear_screen
	end

	def choice_level
		clear_screen
		puts "Wybierz poziom trudności"
		puts "1. Łatwy\n2. Normalny\n3. Trudny"
		
		while true
			print "> "
			level_choice = gets.chomp
			if $levels[level_choice] != nil
				$difficulty_level = $levels[level_choice]
				clear_screen
				break
			end
		end
	end
end

# ---------
# MAIN

menu = Menu.new
load

loop do
	running = true
	
	# menu loop
	loop do
		clear_screen
		menu.show_menu
		print "> "
		option = gets.chomp
		
		if option == "4"
			save
			clear_screen
			exit
		elsif option == "3"
			menu.show_results
		elsif option == "2"
			menu.choice_level
		elsif option == "1"
			break
		else
			next
		end
	end
	
	clear_screen
	game = Game.new($difficulty_level, $rounds, $range)
	game.game

	if game.results.length > 0
		user = set_user
		score = Score.new(user, game.results.sum, game.count_rounds, $difficulty_level)
		$scoreboard.append(score)
	end
end
