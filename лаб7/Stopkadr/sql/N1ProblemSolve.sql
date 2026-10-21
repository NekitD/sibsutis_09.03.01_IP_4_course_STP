SELECT assets.inventory_number, employees.full_name FROM 
assets LEFT JOIN assignments ON (assets.id = assignments.asset_id AND assignments.returned_at IS NULL)
LEFT JOIN employees ON (assignments.employee_id = employees.id);