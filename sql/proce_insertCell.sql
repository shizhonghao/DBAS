create procedure bulk_insert_on
AS
BEGIN
	bulk insert tbCell
	from 'f:\in.txt'
	with
	(fieldterminator=',', 
	rowterminator='\n',
	FIRE_TRIGGERS)
END