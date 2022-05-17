# chmomp() - usuwa znak nowej linii

# sprawdzanie czy wartość podana przez użytkowniak jest liczbą
# mierzenie czasu
# menu gry
# poziomy trudności: przegrana przy zbyt dużej ilości prób zgadnięcia liczby


$times = []
$tries = []

$number_min = 1
$number_max = 128

start = 0
finish = 0

difficulty_level = "normal"
$difficulty_option = {"easy" => 4, "normal" => 2, "hard" => 1}

$user = "anonim"

def show_results()
	puts()
	puts("Wyniki")
	puts("--------")
	puts("Użytkownik #{$user}")
	puts("Liczba gier: #{$tries.count.to_s}")
	puts("Średni wynik: #{(($tries.sum.to_f / $tries.count).round(2)).to_s()}")
	puts("Średni czas: #{(($times.sum.to_f / $times.count).round(2)).to_s()}s")
end

def get_max_choice(level)
	return (Math.log(($number_max - $number_min + 1), 2) * $difficulty_option[level]).to_i
end

def set_user()
	print("Podaj swoje imię: ")
	user = gets.chomp()
	
	
	if user.length() > 0
		return user
	else
		return "anonim"
	end
end

# ---------
# MAIN
loop do
	running = true

	puts("Gra w zgadywanie")
	puts()
	puts("(1)Nowa gra (2)Pokaż wyniki (3)Wyjście")
	
	loop do
		option = gets.chomp()
		
		if option == "3"
			exit
		elsif option == "2"
			show_results()
		elsif option == "1"
			break
		else
			next
		end
	end
	
	
	
	# Choice difficulty level
	puts("Wybierz poziom trudności")
	puts("(1)Łatwy (2)Normalny (3)Trudny")
	loop do
		level_option = gets.chomp()
		
		if level_option == "1"
			difficulty_level = "easy"
		elsif level_option == "2"
			difficulty_level == "normal"
		elsif level_option == "3"
			difficulty_level = "hard"
		else
			next
		end
		
		break
	end
	
	max_choices = get_max_choice(difficulty_level)
	
	
	loop do
		number = rand($number_min..$number_max)
		choice_count = 0
		
		puts("Podaj liczbę w zakresie od #{$number_min} do #{$number_max} | aby zakończyć napisz 'koniec'")
		puts("Maksymalna ilość prób: #{max_choices}")
		puts()

		# game loop
		loop do
			# TBD break if time runs out ------------------------
			choice = gets().chomp()
			
			if choice == 'koniec'
				running = false
				break
			end
			
			#choice = choice.to_i()
			begin
				choice = Integer(choice)
			rescue ArgumentError
				puts('Niepoprawna wartość')
				next
			end
			
			if choice < $number_min or choice > $number_max
				puts('Liczba poza zakresem')
				next
			end
			
			# starts time measure
			if choice_count == 0
				start = Time.now()
			end
			
			choice_count += 1
			
			
			# game logic
			if number > choice
				puts("Więcej..")
			elsif number < choice
				puts("Mniej..")
			else number == choice
				puts("Wygrana!")
				puts("Liczba prób: " + choice_count.to_s())
				break
			end
			
			if choice_count >= max_choices
				puts("Przegrana")
				running = false
				break
			end
		end
		
		if not running
			puts("Gra przerwana!")
			puts()
			break
		end
		
		finish = Time.now()
		diff = finish - start
		
		$times.push(diff)
		$tries.push(choice_count)
		
		
=begin
		puts()
		loop do
			print("Czy chcesz zagrać ponownie? y/n ")
			status = gets().chomp()
			
			if status == "y"
				break
			elsif status == "n"
				running = false
				break
			end
		end
=end
		
		
	end
	
	if $tries.length > 0
		$user = set_user()
	end
end
