class Score
	attr_reader :rounds, :level
	def initialize(name, score, rounds, level)
		@name = name
		@score = score
		@rounds = rounds
		@level = level
	end
	
	def get_rank
		if @level == "trudny" and @rounds >= 6
			"king"
		elsif @level == "trudny" and @rounds >= 4
			"queen"
		elsif @level == "trudny"
			"pawn"
		
		elsif @level == "normalny" and @rounds >= 6
			"rook"
		elsif @level == "normalny" and @rounds >= 4
			"bishop"
		elsif @level == "normalny"
			"pawn"
		
		elsif @level == "łatwy" and @rounds >= 6
			"bishop"
		elsif @level == "łatwy" and @rounds >= 4
			"knight"
		elsif @level == "łatwy"
			"pawn"
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
		#puts @scores
		
		if @scores.length > @size
			remove_last
		end
	end
	
	def sort_by_score
		@scores = @scores.sort_by{ |obj| [obj.level.reverse, -obj.rounds] }
	end
	
	def remove_last
		@scores.delete_at(@scores.length - 1)
	end
	
	def to_s
		ret = "\nWyniki (top #{@size})\n--------\n"
		ret += "Imię | Poziomy | Ranga\n"
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
end

# tests()
