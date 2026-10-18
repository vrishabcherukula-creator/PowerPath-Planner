const plan = {
  total: 14000, naive: 19544,
  buildings: [
    {name: "Substation", x: 0.0, y: 0.0},
    {name: "Library", x: 30.0, y: 40.0},
    {name: "Hostel", x: 60.0, y: 0.0},
    {name: "Lab", x: 30.0, y: 80.0},
  ],
  cables: [
    {u: 1, v: 3, cost: 4000.00, chosen: 1},
    {u: 0, v: 1, cost: 5000.00, chosen: 1},
    {u: 1, v: 2, cost: 5000.00, chosen: 1},
    {u: 0, v: 2, cost: 6000.00, chosen: 0},
    {u: 0, v: 3, cost: 8544.00, chosen: 0},
    {u: 2, v: 3, cost: 8544.00, chosen: 0},
  ]
};
