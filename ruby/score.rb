class Score
	attr_reader :name, :score
	def initialize(name, score)
		@name = name
		@score = score
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
		"#{@name} | #{@score} | #{get_rank}"
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
		ret = "Wyniki (top #{@size})\n--------\n"
		p = 1
		for obj in @scores
			ret += "#{p}. #{obj}\n"
			p += 1
		end
		
		ret
	end
end



# --- TESTS ---
def tests()
	sb = Scoreboard.new(10)
	
	s1 = Score.new("jan", 10)
	s2 = Score.new("piotr", 15)
	s3 = Score.new("tomasz", 12)
	
	sb.append(s1)
	sb.append(s2)
	sb.append(s3)
	
	puts sb
end

#tests()