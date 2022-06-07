class Score
	attr_reader :name, :score
	def initialize(name, score, rounds, level)
		@name = name
		@score = score
		@rounds = rounds
		@level = level
	end
	
	def get_rank
		if @score < 12
			"pawn"
		elsif @score == 12
			"knight"
		else
			"rook"
		end
	end
	
	def to_s
		"#{@name} | #{@rounds} | #{get_rank}"
	end
	
	def to_txt
		return "#{@name},#{@score},#{@rounds},#{@level}"
	end
end

class Scoreboard
	def initialize(size)
		@scores = []
		@size = size
	end
	
	def append(score)
		@scores.append(score)
		
		sort_by_score
		
		if @scores.length > @size
			remove_last
		end
	end
	
	def sort_by_score
		@scores = @scores.sort_by{ |obj| [-obj.score, obj.name] }
	end
	
	def remove_last
		@scores.delete_at(@scores.length - 1)
	end
	
	def to_s
		ret = "\nWyniki (top #{@size})\n--------\n"
		ret += "Imię | Poziomy | Wynik\n"
		p = 1
		for obj in @scores
			ret += "#{p}. #{obj}\n"
			p += 1
		end
		
		ret
	end
	
	def to_txt
		ret = ""
		
		for obj in @scores
			ret += "#{obj.to_txt}\n"
		end
		
		return ret
	end
end



# --- TESTS ---
def tests()
	sb = Scoreboard.new(10)
	
	s1 = Score.new("jan", 10, 2, "normalny")
	s2 = Score.new("piotr", 15, 3, "trudny")
	s3 = Score.new("tomasz", 12, 5, "łatwy")
	
	sb.append(s1)
	sb.append(s2)
	sb.append(s3)
	
	puts sb
	#puts sb.to_txt
end

#tests()